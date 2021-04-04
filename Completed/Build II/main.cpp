#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>

#include <Windows.h>
#include "conio.h"
#include "math.h"
#include "classes.h"
#include "functions.h"

using namespace std;

//WASD + ESC alt code definitions
#define w 119 
#define a 97
#define d 100
#define s 115
#define r 114
#define e 101
#define esc 27

//Console Stuff
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

//Screen height/width DON'T CHANGE!
int screenheight = 40;
int screenwidth = 80;
int screen_size = screenwidth * screenheight;


int main() {

    //Creating console buffer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char *screen = new char[screenwidth*screenheight];
    HANDLE hConsole_c = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole_c);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole_c, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole_c, dwMode);
    DWORD dwBytesWritten = 0;

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hConsole_c, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(hConsole_c, &cursor_info);

	CONSOLE_SCREEN_BUFFER_INFOEX consolesize;
    consolesize.cbSize = sizeof(consolesize);
	GetConsoleScreenBufferInfoEx(hConsole_c, &consolesize);
    consolesize.srWindow.Left = 0;
    consolesize.srWindow.Right = 80;
    consolesize.srWindow.Top = 0;
    consolesize.srWindow.Bottom = 40;
    COORD cw;
    cw.X = 80;
    cw.Y = 40;
    consolesize.dwMaximumWindowSize = cw;
    SetConsoleScreenBufferInfoEx(hConsole_c, &consolesize);
    SetConsoleScreenBufferSize(hConsole_c, cw);

    HWND consoleWindow = GetConsoleWindow(); 
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    //Generating dungeon

    srand(time(NULL));
    
    Dungeon dungeon(80, 40);
    Player player;

    dungeon.init_dungeon(player.playerx, player.playery);

    bool discovered_sectors[9] {false};
    bool discovered_tiles[80 * 40] {false};

    vector<vector<char>> graphic_dungeon = dungeon.return_dungeon();

    int floor = 1;
    
    int wait;

    //Gets key input
    char key = _getch();
    key = tolower(key);
    int key_code = key;

    //Game loop

    bool visible = true;

    while(key_code != esc) {
        SetConsoleTextAttribute(hConsole_c, 0x07);
        SetConsoleCursorPosition(hConsole_c, {0,0});

        // Movement code
        switch(_getch()) {
            case w:
                player.move('n', dungeon);
                break;
            case d:
                player.move('e', dungeon);
                break;
            case s:
                player.move('s', dungeon);
                break;
            case a:
                player.move('w', dungeon);
                break;
            case r:
                if(dungeon.is_move_valid('r', player.playerx, player.playery)) {
                    floor++;
                    dungeon.init_dungeon(player.playerx, player.playery);
                    player.set_cords(player.playerx, player.playery);

                    for(int i = 0; i < sizeof(discovered_sectors); i++) {
                        discovered_sectors[i] = false;
                    }
                    for(int j = 0; j < sizeof(discovered_tiles); j++) {
                        discovered_tiles[j] = false;
                    }
                    graphic_dungeon = dungeon.return_dungeon();
                }
                break;
            case e:
                if(visible) {
                    visible = false;
                } else {
                    visible = true;
                }
                break;
        }
        //Writing buffer to console
        dungeon.print_dungeon(screen, screenwidth, graphic_dungeon, player.playerx, player.playery, discovered_sectors, discovered_tiles, visible);

        sprintf_s(screen, 80,"Player X: %d Player Y: %d Floor: %d\n", player.playerx, player.playery, floor);
        sprintf_s(screen, 80, "Controls: WASD for Movement, E for visibility, R for stairs");


        screen[screenwidth * screenheight - 1] = '\0';
        WriteConsoleA(hConsole_c, screen, screen_size, &dwBytesWritten, NULL);

        COORD playercord = {(SHORT)(player.playerx - 2), (SHORT)player.playery};

        color_tile(hConsole_c, 'b', playercord);

        // Sleep(120);

        key = _getch();
        key = tolower(key);
        key_code = key;
        ungetch(key_code);
    }
}