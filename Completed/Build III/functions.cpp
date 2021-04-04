#include <cstdlib>
#include <conio.h>
#include <math.h>
#include <string>
#include <vector>
#include <windows.h>

#include "functions.h"

using namespace std;

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

int random(int lowerbound, int upperbound) {

    //Upperbound not included in domain of possible numbers
    int rannum = rand() % (upperbound - lowerbound) + lowerbound;
    return rannum;
}

int dist(int x, int y, int x2, int y2) {

    int dx = x2 - x;
    int dy = y2 - y;

    return sqrt(dx*dx + dy*dy);
}

void color_tile(HANDLE& hConsole_c, char colour, COORD cord) {

    int colourcode = 0x00;

    switch(colour) {
        case 'B':
            colourcode = 0x01;
            break;
        case 'b':
            colourcode = 0x09;
            break;
        case 'G':
            colourcode = 0x02;
            break;
        case 'g':
            colourcode = 0x0A;
            break;
        case 'A':
            colourcode = 0x03;
            break;
        case 'a':
            colourcode = 0x0B;
            break;
        case 'R':
            colourcode = 0x04;
            break;
        case 'r':
            colourcode = 0x0C;
            break;
        case 'P':
            colourcode = 0x05;
            break;
        case 'p':
            colourcode = 0x0D;
            break;
        case 'Y':
            colourcode = 0x06;
            break;
        case 'y':
            colourcode = 0x0E;
            break;
        case 'W':
            colourcode = 0x07;
            break;
        case 'w':
            colourcode = 0x0F;
            break;
        case 'd':
            colourcode = 0x08;
            break;
        case 'h':
            colourcode = 0x70;
            break;
    }

    DWORD dw = 1;
    FillConsoleOutputAttribute(hConsole_c, colourcode, 1, {cord.X, cord.Y}, &dw);
}

void color_string(HANDLE hConsole_c, char colour, COORD cord, int length) {
    int offset = cord.X;
    for(int i = 0; i < length; i++) {
        color_tile(hConsole_c, colour, cord);
        cord.X++;
        if(cord.X > 119) {
            cord.Y++;
            cord.X = offset;
        }
    }
}

void toScreen(char*& screen, int column, int row, const char* text) {

    int offset = column;
    for (int i = 0; i < strlen(text); i++) {
        screen[row*120+column] = text[i];
        column++;
        //If column exceeds screen width, wrap text to next row and reset column to original offset
        if(column > 119) {
            row++;
            column = offset;
        }
    }
}

void init_screen(HANDLE hConsole_c) {

    //Disable the blinking cursor
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hConsole_c, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(hConsole_c, &cursor_info);

    //Place the window at top left of primary screen
    HWND consoleWindow = GetConsoleWindow(); 
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //Set window size to equal buffer size
	CONSOLE_SCREEN_BUFFER_INFOEX consolesize;
    consolesize.cbSize = sizeof(consolesize);
	GetConsoleScreenBufferInfoEx(hConsole_c, &consolesize);
    consolesize.srWindow.Left = 0;
    consolesize.srWindow.Right = 119;
    consolesize.srWindow.Top = 0;
    consolesize.srWindow.Bottom = 39;
    COORD cw;
    cw.X = 120;
    cw.Y = 40;
    consolesize.dwMaximumWindowSize = cw;
    SetConsoleScreenBufferInfoEx(hConsole_c, &consolesize);
    SetConsoleScreenBufferSize(hConsole_c, cw);
    SetConsoleWindowInfo(hConsole_c, true, &consolesize.srWindow);

    //Set font to Lucida Console, size 20
    CONSOLE_FONT_INFOEX font;
    font.cbSize = sizeof font;
    font.nFont = 0;
    font.dwFontSize.X = 0;
    font.dwFontSize.Y = 20;
    font.FontFamily = FF_DONTCARE;
    font.FontWeight = FW_DEMIBOLD;
    wcscpy(font.FaceName,  L"Lucida Console");
    SetCurrentConsoleFontEx(hConsole_c, false, &font);

    //Disable console modes like quick edit and insert, disable mouse input.
    DWORD mode;
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &mode); 
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode &= ~ENABLE_INSERT_MODE;
    mode &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(hStdin, mode);

    //Make terminal a virtual terminal
    DWORD dwMode = 0;
    GetConsoleMode(hConsole_c, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole_c, dwMode);

}

void write_screen(char*& screen, HANDLE hConsole_c, int screensize, DWORD dwBytesWritten) {
    screen[screensize] = '\0';
    WriteConsole(hConsole_c, screen, screensize, &dwBytesWritten, NULL);
}

void typing_handler(string& inputstring, int& cursor, int limit, int &keypressed) {
    if((GetAsyncKeyState(VK_BACK) & (0x8000 != 0)) && cursor > 0) {
        bool deleted = false;
        if(cursor >= inputstring.length()) {
            inputstring.pop_back();
            deleted = true;
        } else {
            inputstring.erase(inputstring.begin()+cursor-1);
            deleted = true;
        }
        if(deleted) {
            cursor--;
        }
        keypressed = 1;
        return;
    } else if((GetAsyncKeyState(VK_LEFT) & (0x8000 != 0)) && cursor > 0) {
        cursor--;
        keypressed = 1;
        return;
    } else if((GetAsyncKeyState(VK_RIGHT) & (0x8000 != 0)) && cursor < inputstring.length()) {
        cursor++;
        keypressed = 1;
        return;
    } else {
        for(int i = 0x41; i < 0x5B; i++) {
            if(GetAsyncKeyState(i) & (0x8000 != 0) || GetAsyncKeyState(VK_SPACE) & (0x8000 != 0)) {
                char ikey = i;
                if(GetAsyncKeyState(VK_SPACE) & (0x8000 != 0)) {
                    ikey = ' ';
                }
                if(!GetAsyncKeyState(VK_SHIFT) & (0x8000 != 1)) {
                    ikey = tolower(ikey);
                }
                if(cursor >= limit) {
                    cursor = limit;
                }
                if(cursor == inputstring.length()) {
                    inputstring.push_back(ikey);
                    cursor++;
                } else {
                    inputstring[cursor] = ikey;
                    cursor++;
                }
                keypressed = 1;
                return;
            }
        }
    }
}

void display_ui(char*& screen, HANDLE hConsole_c, Player player) {

    string playerh = to_string(player.health).append(" (").append(to_string(player.maxhealth)).append(")");
    string playerd = to_string(player.defense);
    string playere = to_string(player.exp).append(" (").append(to_string(player.expperlvl).append(")"));
    string playerl = to_string(player.level);
    string playerg = to_string(player.gold);

    string playerstring = player.playername.c_str();

    string headers = "===============";

    int addoffset = 1;

    if(playerstring.length() % 2 == 0) {
        headers.append("=");
        addoffset = 0;
    } else {
        headers.append("==");
    }

    int drawpos = 8 - (playerstring.length()/2);

    //Player name
    toScreen(screen, 86, 4, headers.c_str());
    toScreen(screen, 86, 5, "=");
    toScreen(screen, 86+drawpos, 5, playerstring.c_str());
    toScreen(screen, 101+addoffset, 5, "=");
    toScreen(screen, 86, 6, headers.c_str());

    //Health
    toScreen(screen, 86, 8, "Health:");
    toScreen(screen, 97, 8, playerh.c_str());
    
    //Defense
    toScreen(screen, 86, 10, "Defense:");
    toScreen(screen, 97, 10, playerd.c_str());
    
    //Experience Points
    toScreen(screen, 86, 12, "Exp:");
    toScreen(screen, 97, 12, playere.c_str());

    //Level
    toScreen(screen, 86, 14, "Level:");
    toScreen(screen, 97, 14, playerl.c_str());

    //Gold
    toScreen(screen, 86, 16, "Gold:");
    toScreen(screen, 97, 16, playerg.c_str());
}


void display_mainmenu(char*& screen, HANDLE hConsole_c, int cursor, int screensize, DWORD dwBytesWritten) {
    
    //Display Menu Options
    char options[3][11] = {"Start", "Build Info", "Quit"};
    for(int i = 0; i < 3; i++) {
        toScreen(screen, 14, 3*i+10, options[i]);
    }
    toScreen(screen, 14, 30, "Copyright (c) Tristan Eggenberger 2021");

    //Display menu cursor;
    toScreen(screen, 10, 3*cursor+10, "->");
    write_screen(screen, hConsole_c, screensize, dwBytesWritten);
    color_string(hConsole_c, 'h', {14, (SHORT)(3*cursor+10)}, strlen(options[cursor]));
}

void display_build(char*& screen, HANDLE hConsole_c) {

    string comptime = "Compiled On: ";
    comptime.append(__DATE__);

    toScreen(screen, 14, 10, "Build Info:");
    toScreen(screen, 14, 13, "Roguelike Build 3 Development Preview");
    toScreen(screen, 14, 16, comptime.c_str());
    toScreen(screen, 14, 19, "THIS IS A PREVIEW BUILD, ISSUES ARE NOT INDICATIVE OF FINAL PRODUCT");
    toScreen(screen, 14, 22, "DO NOT DISTRIBUTE!");
}

void display_gamemenu(char*& screen, HANDLE hConsole_c, int cursor, int screensize, DWORD dwBytesWritten) {

    //Display Menu Options
    char options[4][19] {"Continue", "Inventory", "Quit to Main Menu", "Quit to Desktop"};
    for(int i = 0; i < 4; i++) {
        toScreen(screen, 14, 3*i+10, options[i]);
    }

    //Display menu cursor;
    toScreen(screen, 10, 3*cursor+10, "->");
    COORD cursorcord;
    cursorcord.X = (SHORT)14;
    cursorcord.Y = (SHORT)(3*cursor+10);
    write_screen(screen, hConsole_c, screensize, dwBytesWritten);
    color_string(hConsole_c, 'h', cursorcord, strlen(options[cursor]));
}

void display_deathscreen(char*& screen, HANDLE hConsole_c, Player player) {
    

    string playerstring = player.playername.c_str();
    string playerg = "Gold: ";
    string rip = "R.I.P.";
    string header = "----------------";
    string header2 = "______________";

    playerg.append(to_string(player.gold));

    int addoffset = 1;

    if(playerstring.length() % 2 == 0) {
        header.append("-");
        header2.append("_");
        addoffset = 0;
    } else {
        header.append("--");        
        header2.append("__");
    }

    int drawpos = 8 - (playerstring.length()/2);
    int drawpos2 = 8 - (rip.length()/2);
    int drawpos3 = 8 - (playerg.length()/2);
    header2.pop_back();
    header2.erase(header2.begin());
    rip.pop_back();

    //Grave Stone
    toScreen(screen, 50, 9, "You Died!");
    toScreen(screen, 47, 11, header2.c_str());
    toScreen(screen, 46, 12, "//");
    toScreen(screen, 60+addoffset, 12, "\\");
    toScreen(screen, 45, 13, "||");
    toScreen(screen, 61+addoffset, 13, "|");
    toScreen(screen, 45, 14, "||");
    toScreen(screen, 61+addoffset, 14, "|");
    toScreen(screen, 47+drawpos2, 14, rip.c_str());
    toScreen(screen, 45, 15, "||");
    toScreen(screen, 61+addoffset, 15, "|");
    toScreen(screen, 45, 16, "||");
    toScreen(screen, 61+addoffset, 16, "|");
    toScreen(screen, 45, 17, "||");
    toScreen(screen, 46+drawpos, 17, playerstring.c_str());
    toScreen(screen, 61+addoffset, 17, "|");
    toScreen(screen, 45, 18, "||");
    toScreen(screen, 61+addoffset, 18, "|");
    toScreen(screen, 45, 19, "||");
    toScreen(screen, 61+addoffset, 19, "|");
    toScreen(screen, 45, 20, "||");
    toScreen(screen, 46+drawpos3, 20, playerg.c_str());
    toScreen(screen, 61+addoffset, 20, "|");
    toScreen(screen, 45, 21, "||");
    toScreen(screen, 61+addoffset, 21, "|");
    toScreen(screen, 45, 22, "||");
    toScreen(screen, 61+addoffset, 22, "|");
    toScreen(screen, 45, 23, header.c_str());

}

void colour_objects(char*& screen, int screenwidth, int screenheight, HANDLE hConsole_c) {
    for(int i = 0; i < screenheight; i++) {
        for(int j = 0; j < 80; j++) {
            COORD cord = {(SHORT)(j), (SHORT)i};
            switch(screen[screenwidth*i+j]) {
                case '@':
                    color_tile(hConsole_c, 'b', cord);
                    break;
                case '$':
                    color_tile(hConsole_c, 'Y', cord);
                    break;
                case '!':
                    color_tile(hConsole_c, 'p', cord);
                    break;
                case 'B':
                    color_tile(hConsole_c, 'd', cord);
                    break;
                case 'G':
                    color_tile(hConsole_c, 'g', cord);
                    break;
                case 'S':
                    color_tile(hConsole_c, 'G', cord);
                    break;
            }
        }
    }
}

int return_sector(int x, int y) {

    int sector;
    int column, row;

    if(x < 29) {
        column = 0;
    } else {
        column = (x - 29) / 26 + 1;
    }
    if(y < 17) {
        row = 0;
    } else {
        row = (y - 17) / 9 + 1;
    }
    sector = row*3+column;
    return sector;
}

int return_roomcord(int roomsector, bool x) {
    int row;
    if(roomsector < 3) {
        row = 0;
    } else if(roomsector < 6) {
        row = 1;
    } else {
        row = 2;
    }
    if(!x) { 
        return row;
    }
    return roomsector - row*3;
}

bool can_connect(int startsector, int endsector, int sectors[9], bool& horizontal) {

    int startx,starty,endx,endy;
    int dx,dy;

    if(startsector == endsector || sectors[startsector] == 0 || sectors[endsector] == 0) {
        return false;
    }

    startx = return_roomcord(startsector, true);
    starty = return_roomcord(startsector, false);

    endx = return_roomcord(endsector, true);
    endy = return_roomcord(endsector, false);

    dx = endx - startx;
    dy = endy - starty;

    if(dy == 0) {
        horizontal = true;
    } else if(dx == 0) {
        horizontal = false;
    } else {
        return false;
    }

    if(horizontal) {
        for(int x = startx; x != endx; x += dx/abs(dx)) {
            if(x != startx && sectors[3*starty+x] == 1) {
                return false;
            }
        }
    } else {
        for(int y = starty; y != endy; y += dy/abs(dy)) {
            if(y != starty && sectors[3*y+startx] == 1) {
                return false;
            }
        }
    }
    return true;
}

bool have_connected(vector<vector<int>> connections, int startsector, int endsector) {

    for(int i = 0; i < sizeof(connections[startsector]); i++) {
        if(connections[startsector][i] == endsector) {
            return true;
        }
    }
    return false;
}

void update_enemies(vector<Enemy*> &enarr, Dungeon dungeon, Player &player, int turns_since_hit) {

    for(int i = 0; i < enarr.size(); i++) {
        if(enarr[i]->has_died()) {
            delete enarr[i];
            enarr.erase(enarr.begin()+i);
            player.exp += random(1,4);
        } else {
            if(enarr[i]->can_attack(player)) {
                player.attack(enarr[i]->get_damage()+random(enarr[i]->get_damage()/-1,1));
                turns_since_hit = 0;
            } else {
                enarr[i]->move(dungeon, player, enarr);
            }
        }
    }
}

void initialize_enemies(Dungeon dungeon, vector<Enemy*> &enarr, Player player, vector<int> sectors) {

    int randx,randy;
    int enemyid;

    int px = player.playerx;
    int py = player.playery;

    vector<vector<char>> map = dungeon.return_dungeon();

    for(int i = 0; i < sectors.size(); i++) {
        if(return_sector(px,py) == sectors[i]) {
            i++;
        }
        int enemy_limit = random(1,3);
        if(random(0,10) == 2) {
            enemy_limit = 0;
        }
        for(int j = 0; j < enemy_limit; j++) {
            randx = 0;
            randy = 0;
            enemyid = random(0,3);
            while(map[randy][randx] != '.' || return_sector(randx,randy) != sectors[i]) {
                randx = random(0,80);
                randy = random(0,40);
            }
            Enemy* en = new Enemy(enemyid, randx, randy);
            enarr.push_back(en);
        }
    }
}

bool is_enemy(vector<Enemy*> &enarr, Player player, int x, int y) {
    for(int i = 0; i < enarr.size(); i++) {
        if(enarr[i]->getx() == x && enarr[i]->gety() == y) {
            return true;
        }
    }
    return false;
}

bool occupied_enemy(vector<Enemy*> &enarr, Player player, char direction, int &enid) {

    int xdir = player.playerx;
    int ydir = player.playery;

    switch(direction) {
        case 'n':
            ydir--;
            break;
        case 'e':
            xdir++;
            break;
        case 's':
            ydir++;
            break;
        case 'w':
            xdir--;
            break;
    }

    for(int i = 0; i < enarr.size(); i++) {
        if(enarr[i]->getx() == xdir && enarr[i]->gety() == ydir) {
            enid = i;
            return true;
        }
    }
    return false;
}

void initialize_potions(Dungeon dungeon, vector<Potion*> &potionarr, vector<int> sectors) {
    int randx,randy,limit;
    int type;

    for(int i = 0; i < sectors.size(); i++) {
        limit = random(0,3);
        vector<vector<char>> map = dungeon.return_dungeon();
        for(int j = 0; j < limit; j++) {
            randx = 0;
            randy = 0;
            while(map[randy][randx] != '.' || return_sector(randx, randy) != sectors[i]) {
                randx = random(0,80);
                randy = random(0,40);
                for(int h = 0; h < potionarr.size(); h++) {
                    if(potionarr[h]->xcord == randx && potionarr[h]->ycord == randy) {
                        randx = random(0,80);
                        randy = random(0,80);
                    }
                }
            }
            
            int type = 0;
            if(random(0,11) < 2) {
                type = 1;
            }
            Potion* po = new Potion(randx, randy, type);
            potionarr.push_back(po);
        }
    }
}

void update_potion(vector<Potion*> &potionarr, Player &player) {
    for(int i = 0; i < potionarr.size(); i++) {
        if(potionarr[i]->xcord == player.playerx && potionarr[i]->ycord == player.playery) {
            switch(potionarr[i]->type) {
                case 0:
                    player.gold += potionarr[i]->value;
                    break;
                case 1:
                    player.heal(potionarr[i]->value);
                    break;
            }
            delete potionarr[i];
            potionarr.erase(potionarr.begin() + i);
            break;
        }
    }
}

void update_objects(vector<Enemy*> &enarr, vector<Potion*> &potionarr, Player &player, Dungeon dungeon) {
    
}