#include <cstdlib>
#include <ctime>
#include "math.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <winuser.h>

#include "gameclasses.h"
#include "functions.h"

//Screen height/width DON'T CHANGE!
int screenheight = 40;
int screenwidth = 120;
int screen_size = screenwidth * screenheight;

int main() {

    //Creating console buffer
    char *screen = new char[screenwidth*screenheight];
    memset(screen, ' ', screen_size);
    HANDLE hConsole_c = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole_c);
    DWORD dwBytesWritten = 0;

    init_screen(hConsole_c);

    HWND cwindow = GetForegroundWindow();

    bool playing = true, mainmenu = true;
    bool ingame = false, ininv = false, inmenu = false, inbuild = false, instart = false, dead = false;

    int cursor = 0;    
    bool keypressed = true;
    bool written = false;
    int pressedkey = 0;
    bool hasdisplayed = false;

    bool visible = false;

    srand(time(NULL));

    Dungeon dungeon(80, 40);
    Player player;

    vector<int> roomsectors;
    vector<vector<char>> graphic_dungeon;
    vector<Enemy*> enarr;
    vector<Potion*> potionarr;

    int floor = 1;
    int turncount = 0;
    int prevturncount = 0;
    int turns_since_hit = 0;
    int turns_since_heal = 0;

    string response;

    while(playing) {

        while(mainmenu) {
            response.clear();
            written = false;
            if(keypressed) {
                if(cursor > 2) {
                    cursor = 0;
                }
                if(cursor < 0) {
                    cursor = 2;
                }
                memset(screen, ' ', screen_size);
                display_mainmenu(screen, hConsole_c, cursor, screen_size, dwBytesWritten);
                GetAsyncKeyState(VK_UP);
                GetAsyncKeyState(VK_DOWN);
                keypressed = false;
            }

            hasdisplayed = false;
            if((GetAsyncKeyState(VK_UP) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                cursor--;
                keypressed = true;
            } else if((GetAsyncKeyState(VK_DOWN) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                cursor++;
                keypressed = true;
            } else if((GetAsyncKeyState(VK_RETURN) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                keypressed = true;
                mainmenu = false;
                switch(cursor) {
                    case 0:
                        //Initializing Dungeon and Player
                        GetAsyncKeyState(VK_RETURN);
                        instart = true;
                        dead = false;
                        cursor = 0;
                        break;
                    case 1:
                        inbuild = true;
                        break;
                    case 2:
                        playing = false;
                        break;
                }
            }
        }

        while(instart) {
            if(GetAsyncKeyState(VK_ESCAPE) & (0x8000 != 0) && cwindow == GetForegroundWindow()) {
                instart = false;
                mainmenu = true;
                keypressed = true;
                cursor = 0;
                break;
            }
            if(!written) {
                memset(screen, ' ', screen_size);
                toScreen(screen, 14, 14, "Enter Player Name: ");
                write_screen(screen, hConsole_c, screen_size, dwBytesWritten);
                written = true;
            }
            pressedkey = 0;
            if(cwindow == GetForegroundWindow()) {
                typing_handler(response, cursor, 13, pressedkey);
            }
            if(pressedkey == 1) {
                memset(screen, ' ', screen_size);
                toScreen(screen, 14, 14, "Enter Player Name: ");
                toScreen(screen, 36, 14, response.c_str());
                toScreen(screen, cursor+35, 15, "^");
                write_screen(screen, hConsole_c, screen_size, dwBytesWritten);
                color_tile(hConsole_c, 'h', {(SHORT)(cursor+35), 14});
            }
            if((GetAsyncKeyState(VK_RETURN) & (0x8000 != 0)) && response.length() >= 4 && cwindow == GetForegroundWindow()) {
                if(response[response.size()-1] == ' ') {
                    response.pop_back();
                }
                player.set_name(response.c_str());
                player.reset_stats();

                for(int i = 0; i < enarr.size(); i++) {
                    delete enarr[i];
                }
                enarr.clear();
                for(int j = 0; j < potionarr.size(); j++) {
                    delete potionarr[j];
                }
                potionarr.clear();

                roomsectors.clear();
                dungeon.init_dungeon(player.playerx, player.playery, roomsectors);
                player.reset_map();
                turns_since_hit = 0;
                turns_since_heal = 0;

                graphic_dungeon = dungeon.return_dungeon();
                
                initialize_enemies(dungeon, enarr, player, roomsectors);
                initialize_potions(dungeon, potionarr, roomsectors);

                cursor = 0;

                ingame = true;
                instart = false;
            }
        }

        while(inbuild) {
            if(GetAsyncKeyState(VK_RETURN) & (0x8000 != 0) && cwindow == GetForegroundWindow()) {
                mainmenu = true;
                inbuild = false;
                keypressed = true;
                break;
            }
            if(!hasdisplayed) {
                memset(screen, ' ', screen_size);
                display_build(screen, hConsole_c);
                toScreen(screen, 14, 30, "Press Enter to return to Main Menu");
                write_screen(screen, hConsole_c, screen_size, dwBytesWritten);
                hasdisplayed = true;
            }
        }

        while(ingame) {

            SetConsoleTextAttribute(hConsole_c, 0x07);
            SetConsoleCursorPosition(hConsole_c, {0,0});

            if(keypressed) {
                if(turncount != prevturncount) {
                    turns_since_hit++;
                    update_enemies(enarr, dungeon, player, turns_since_hit);    
                    if(turns_since_hit == 0) {
                        turns_since_heal = 0;
                    }
                    if(turns_since_hit >= 20) {
                        turns_since_heal++;
                        if(turns_since_heal >= 10) {
                            player.heal(1);
                            turns_since_heal = 0;
                        }
                    }
                }
                memset(screen, ' ', screen_size);
                dungeon.print_dungeon(screen, screenwidth, graphic_dungeon, player, enarr, potionarr, visible);
                if(player.exp >= player.expperlvl) {
                    player.levelup();
                    player.exp = 0;
                };

                display_ui(screen, hConsole_c, player);

                write_screen(screen, hConsole_c, screen_size, dwBytesWritten);
                colour_objects(screen, screenwidth, screenheight, hConsole_c);
            }

            if(player.health <= 0) {
                dead = true;
                hasdisplayed = false;
                ingame = false;
                break;
            }

            keypressed = false;
            prevturncount = turncount;

            // Movement code
            int enid;
            char direction = ' ';
            if((GetAsyncKeyState(VK_UP) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                direction = 'n';
                keypressed = true;
            }
            if((GetAsyncKeyState(VK_RIGHT) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                direction = 'e';
                keypressed = true;
            }
            if((GetAsyncKeyState(VK_DOWN) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) { 
                direction = 's';
                keypressed = true;
            }
            if((GetAsyncKeyState(VK_LEFT) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                direction = 'w';
                keypressed = true;
            }
            if((GetAsyncKeyState('R') & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                if(dungeon.is_move_valid('r', player.playerx, player.playery)) {
                    floor++;
                    for(int i = 0; i < enarr.size(); i++) {
                        delete enarr[i];
                    }
                    enarr.clear();
                    for(int j = 0; j < potionarr.size(); j++) {
                        delete potionarr[j];
                    }
                    potionarr.clear();

                    roomsectors.clear();
                    dungeon.init_dungeon(player.playerx, player.playery, roomsectors);
                    player.set_cords(player.playerx, player.playery);

                    player.reset_map();

                    graphic_dungeon = dungeon.return_dungeon();
                    initialize_enemies(dungeon, enarr, player, roomsectors);
                    initialize_potions(dungeon, potionarr, roomsectors);
                    keypressed = true;
                }
            }
            if((GetAsyncKeyState(VK_SPACE) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                keypressed = true;
                direction = 'p';
            }
            if((GetAsyncKeyState(VK_ESCAPE) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                inmenu = true;
                cursor = 0;
                keypressed = true;
            }

            if(direction != ' ') {
                if(occupied_enemy(enarr, player, direction, enid)) {
                    enarr[enid]->attack(player.damage);
                } else {
                    player.move(direction, dungeon);
                }
                turncount++;
                update_potion(potionarr, player);
            }

            while(inmenu) {
                if(keypressed) {
                    memset(screen, ' ', screen_size);
                    display_gamemenu(screen, hConsole_c, cursor, screen_size, dwBytesWritten);
                    keypressed = false;
                    GetAsyncKeyState(VK_UP);
                    GetAsyncKeyState(VK_DOWN);
                }
                if((GetAsyncKeyState(VK_UP) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                    cursor--;
                    keypressed = true;
                }
                if((GetAsyncKeyState(VK_DOWN) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                    cursor++;
                    keypressed = true;
                }

                if(cursor < 0) {
                    cursor = 3;
                }
                if(cursor > 3) {
                    cursor = 0;
                }

                if((GetAsyncKeyState(VK_RETURN) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                    keypressed = true;
                    switch(cursor) {
                        case 0:
                            inmenu = false;
                            break;
                        case 1:
                            break;
                        case 2:
                            cursor = 0;
                            mainmenu = true;
                            ingame = false;
                            inmenu = false;
                            break;
                        case 3:
                            inmenu = false;
                            ingame = false;
                            playing = false;
                            break;
                    }
                }
                if((GetAsyncKeyState(VK_ESCAPE) & (0x8000 != 0)) && cwindow == GetForegroundWindow()) {
                    inmenu = false;
                    keypressed = true;
                    cursor = 0;
                }
            }
        }
        while(dead) {
            if(GetAsyncKeyState(VK_RETURN) & (0x8000 != 0) && cwindow == GetForegroundWindow()) {
                mainmenu = true;
                dead = false;
                keypressed = true;
                break;
            }
            if(!hasdisplayed) {
                memset(screen, ' ', screen_size);
                display_deathscreen(screen, hConsole_c, player);
                toScreen(screen, 37, 30, "Press Enter to return to Main Menu");
                write_screen(screen, hConsole_c, screen_size, dwBytesWritten);
                hasdisplayed = true;
            }
        }
    }

    return 0;
}