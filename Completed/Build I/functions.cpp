#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <windows.h>

#include "functions.h"

using namespace std;

void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD cord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, cord);
}

void showConsoleCursor(bool show) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    
    GetConsoleCursorInfo(out, &cursor_info);
    cursor_info.bVisible = show;
    SetConsoleCursorInfo(out, &cursor_info);
}

int random(int lowerbound, int upperbound) {

    int rannum = rand() % (upperbound - lowerbound) + lowerbound;
    return rannum;
}

void toScreen(char*& screen, const int index, const char* text) {
    for (int i = 0; i < strlen(text); i++) {
        screen[index+i] = text[i];
    }
}

int return_sector(int x, int y) {
    int sector;
    if(x < 29) {
        if(y < 17) {
            sector = 0;
        } else if(y < 26) {
            sector = 3;
        } else {
            sector = 6;
        }
    } else if(x < 55) {
        if(y < 17) {
            sector = 1;
        } else if(y < 26) {
            sector = 4;
        } else {
            sector = 7;
        }
    } else {
        if(y < 17) {
            sector = 2;
        } else if(y < 26) {
            sector = 5;
        } else {
            sector = 8;
        }
    }

    return sector;
}

bool can_connect(int startsector, int endsector, int sectors[9], bool& xfirst) {
    int startx,starty,endx,endy;
    int dx,dy;

    if(startsector == endsector) {
        return false;
    }

    if(sectors[startsector] == 0 || sectors[endsector] == 0) {
        return false;
    }

    if(startsector % 3 == 0) {
        startx = 0;
        starty = startsector / 3;
    } else if((startsector - 1) % 3 == 0) {
        startx = 1;
        starty = (startsector - 1) / 3;
    } else if((startsector - 2) % 3 == 0) {
        startx = 2;
        starty = (startsector - 2) / 3;
    }

    if(endsector % 3 == 0) {
        endx = 0;
        endy = endsector / 3;
    } else if((endsector - 1) % 3 == 0) {
        endx = 1;
        endy = (endsector - 1) / 3;
    } else if((endsector - 2) % 3 == 0) {
        endx = 2;
        endy = (endsector - 2) / 3;
    }

    dx = endx - startx;
    dy = endy - starty;

    for(int x = startx; x != endx; x += dx/abs(dx)) {
        if(x != startx && sectors[3*starty+x] == 1) {
            xfirst = false;
        }
    }
    for(int y = starty; y != endy; y += dy/abs(dy)) {
        if(sectors[3*y+endx] == 1 && y != endy) {
            xfirst = false;
        }
    }
    if(xfirst) {
        return true;
    } else {
        for(int y = starty; y != endy; y += dy/abs(dy)) {
            if(y != starty && sectors[3*y+startx] == 1) {
                return false;
            }
        }
        for(int x = startx; x != endx; x += dx/abs(dx)) {
            if(sectors[3*endy+x] == 1) {
                return false;
            }
        }
        return true;
    }
}

void color_tile(HANDLE& hConsole_c, char colour, COORD cord) {

    int colourcode;

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
    }

    DWORD dw = 1;
    FillConsoleOutputAttribute(hConsole_c, colourcode, 1, {cord.X, cord.Y}, &dw);
}