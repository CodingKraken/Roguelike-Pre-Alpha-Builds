#include <string>
#include <iostream>
#include <vector>
#include <windows.h>

#include "classes.h"
#include "functions.h"

using namespace std;

Dungeon::Dungeon(int len, int ht) {
    length = len;
    height = ht;

    dungeon.resize(height);
    for(int i = 0; i < height; i++) {
        dungeon[i].resize(length);
        for(int j = 0; j < length; j++) {
            dungeon[i][j] = ' ';
        }
    }
};

void Dungeon::init_room(int xpos, int ypos, int roomlength, int roomheight, bool is_exitroom) {

    if(ypos + roomheight > height - 1) {
        roomheight = height - ypos;
    }

    if(xpos + roomlength > length - 1) {
        roomlength = length - xpos;
    }

    int exitx,exity;

    if(is_exitroom) {
        exitx = random(2,roomlength - 2) + xpos;
        exity = random(2,roomheight - 2) + ypos;
    }

    for(int y = ypos; y < roomheight + ypos; y++) {
        for(int x = xpos; x < xpos + roomlength; x++) {
            if(y == ypos || y == ypos + roomheight - 1) {
                dungeon[y][x] = '-';
            } else if(x == xpos || x == xpos + roomlength - 1) {
                dungeon[y][x] = '|';
            } else if(is_exitroom && (x == exitx && y == exity)) {
                dungeon[y][x] = '%';
            } else {
                dungeon[y][x] = '.';
            }
        }
    }
}

void Dungeon::init_corridors(int startsector, vector<int>& connections, int sectors[9], int xoffset, int yoffset) {

    int targetsector = random(0,9);

    while(sectors[targetsector] == 0 || targetsector == startsector || connections[startsector] == targetsector || connections[targetsector] == startsector || (connections[startsector] != 9 && connections[startsector] == connections[targetsector])) {
        if(!(sectors[targetsector] == 0 || targetsector == startsector || connections[startsector] == targetsector || connections[targetsector] == startsector && (connections[startsector] != 9 && connections[startsector] == connections[targetsector]))) {
            return;
        }
        targetsector = random(0,9);
    }

    connections[startsector] = targetsector;
    connections[targetsector] = startsector;

    int startx,starty,endx,endy;
    int dx,dy;

    if(startsector % 3 == 0) {
        startx = 7 + xoffset;
        starty = ((startsector / 3) * 9 + 9 + yoffset);
    } else if((startsector - 1) % 3 == 0) {
        startx = 30 + xoffset;
        starty = (((startsector - 1) / 3) * 9 + 9 + yoffset);
    } else if((startsector - 2) % 3 == 0){
        startx = 56 + xoffset;
        starty = (((startsector - 2) / 3) * 9 + 9 + yoffset);
    }

    if(targetsector % 3 == 0) {
        endx = 7 + xoffset;
        endy = ((targetsector / 3) * 9 + 9 + yoffset);
    } else if((targetsector - 1) % 3 == 0) {
        endx = 30 + xoffset;
        endy = (((targetsector - 1) / 3) * 9 + 9 + yoffset);
    } else if((targetsector - 2) % 3 == 0){
        endx = 56 + xoffset;
        endy = (((targetsector - 2) / 3) * 9 + 9 + yoffset);
    }

    dx = endx - startx;
    dy = endy - starty;

    int xrate,yrate;

    if(dx < 0) {
        xrate = -1;
    } else {
        xrate = 1;
    }

    bool has_shifted = false;
    int coinflip;

    for(int x = startx; x != endx; x += xrate) {
        /* Reworked Corridor Algorithm
        coinflip = random(0,2);
        if(coinflip == 1 && dungeon[starty][x] == ' ' && !has_shifted && dungeon[starty+1][x] != '#' && dungeon[starty-1][x] != '#') {
            if(!has_shifted) {
                int randychange = random(1,3);
                for(int i = starty; i != randychange+starty; i++) {
                    if(dungeon[i+1][x+1] == '#' || dungeon[i-1][x-1] == '#') {
                        dungeon[i][x] = ' ';
                    }
                    if(dungeon[i][x] == ' ') {
                        dungeon[i][x] = '#';
                    }
                }
                starty += randychange;
                endy += randychange;
                has_shifted = true;
            }
        }*/

        if(dungeon[starty][x] == '|' || (dungeon[starty][x] != '.' && dungeon[starty][x] != '#' && dungeon[starty][x] != '%' && dungeon[starty][x] != ' ' && dungeon[starty-1][x] != '+' && dungeon[starty+1][x] != '+')) {
            dungeon[starty][x] = '+';
        } else if(dungeon[starty][x] != '.' && dungeon[starty][x] != '%' && dungeon[starty][x] != '|') {
            dungeon[starty][x] = '#';
        }
    }

    if(dy < 0) {
        yrate = -1;
    } else {
        yrate = 1;
    }

    has_shifted = false;

    for(int y = starty; y != endy; y += yrate) {
    /* Reworked Corridor Algorithm
        coinflip = random(0,2);
        if(coinflip == 1 && dungeon[y][endx] == ' ' && !has_shifted && dungeon[y][endx+1] != '#' && dungeon[y][endx-1] != '#') {
            if(!has_shifted) {
                int randxchange = random(1,4);
                for(int i = endx; i != randxchange+endx; i++) {
                    if(dungeon[y+1][i+1] == '#' || dungeon[y-1][i-1] == '#') {
                        dungeon[y][i] = ' ';
                    }
                    if(dungeon[y+1][i] == '#' || dungeon[y-1][i] == '#') {
                        dungeon[y][i] = '#';
                    }
                    if(dungeon[y][i] == ' ') {
                        dungeon[y][i] = '#';
                    }
                }
                endx += randxchange;
                has_shifted = true;
            }
        }*/
        if(dungeon[y+1][endx] == '-' && dungeon[y][endx] != '%' && dungeon[y][endx] != '.') {
            dungeon[y+1][endx] = '+';
            dungeon[y][endx] = '#';
        } else if(dungeon[y-1][endx] == '-' && dungeon[y][endx] != '%' && dungeon[y][endx] != '.') {
            dungeon[y-1][endx] == '+';
            dungeon[y][endx] = '#';
        } else if(dungeon[y][endx] == '-') {
            dungeon[y][endx] = '+';
        } else if(dungeon[y-1][endx] == '-') {
            dungeon[y-1][endx] == '+';
        } else if(dungeon[y+1][endx] == '-') {
            dungeon[y+1][endx] == '-';
        } else if(dungeon[y][endx] == ' ') {
            dungeon[y][endx] = '#';
        }
    }
}

void Dungeon::init_dungeon(int& playerx, int& playery) {

    for(int i = 0; i < height; i++) {
        dungeon[i].resize(length);
        for(int j = 0; j < length; j++) {
            dungeon[i][j] = ' ';
        }
    }

    int limit = random(6,8);
    int ranxpos,ranypos,ranlength,ranheight;
    int sectors[9] = {0};

    int randexit = random(1, limit);

    for(int i = 0; i < limit; i++) {
        int roomsector = random(0,8);
        if(i == limit - 1) {
            if(sectors[0] == 0 && sectors[3] == 0 && sectors[6] == 0) {
                roomsector = random(0,4) * random(0,3);
            } else if(sectors[1] == 0 && sectors[4] == 0 && sectors[7] == 0) {
                roomsector = random(0,4) * random(0,3)+1;
            } else if(sectors[2] == 0 && sectors[5] == 0 && sectors[8] == 0){
                roomsector = random(0,4) * random(0,3)+2;
            } else if(sectors[0] == 0 && sectors[1] == 0 && sectors[2] == 0){
                roomsector = random(0,3);
            } else if(sectors[3] == 0 && sectors[4] == 0 && sectors[5] == 0){
                roomsector = random(3,6);
            } else if(sectors[6] == 0 && sectors[7] == 0 && sectors[8] == 0){
                roomsector = random(6,9);
            }
        }

        while(sectors[roomsector] != 0) {
            roomsector = random(0,8);
        }

        if(roomsector % 3 == 0) {
            ranxpos = 5;
            ranypos = ((roomsector / 3) * 9 + 8);
        } else if((roomsector - 1) % 3 == 0) {
            ranxpos = 29;
            ranypos = (((roomsector - 1) / 3) * 9 + 8);
        } else if((roomsector - 2) % 3 == 0){
            ranxpos = 55;
            ranypos = (((roomsector - 2) / 3) * 9 + 8);
        }

        if(i == 0) {
            playerx = ranxpos + random(2,8);
            playery = ranypos + 2;
        }

        sectors[roomsector] = 1;

        ranlength = random(12,22);
        ranheight = random(6, 8);

        if(i == randexit) {
            init_room(ranxpos,ranypos,ranlength,ranheight, true);
        } else {
            init_room(ranxpos,ranypos,ranlength,ranheight, false);
        }
    }

    int randx = random(0, 9);
    int randy = random(0, 4);


    int randomstart = random(0,8);
    vector<int> hasconnections(9,9);
    vector<int> hasdrawnfrom (9,0);
    for(int j = 0; j < limit; j++) {
        while(hasdrawnfrom[randomstart] != 0 || sectors[randomstart] == 0) {
            randomstart = random(0,9);
        }
        init_corridors(randomstart, hasconnections, sectors, randx, randy);
        hasdrawnfrom[randomstart] = 1;
    }
}

bool Dungeon::is_move_valid(char direction, int playerx, int playery) {

    int north = playery - 1;
    int east = playerx + 1;
    int south = playery + 1;
    int west = playerx - 1;

    bool can_move = true;

    switch(direction) {
        case 'n':
            if(playery == 0 || dungeon[north][playerx] == '-' || dungeon[north][playerx] == '|' || dungeon[north][playerx] == ' ') {
                can_move = false;
            }
            break;
        case 'e':
            if(playerx == length - 1 || dungeon[playery][east] == '-' || dungeon[playery][east] == '|' || dungeon[playery][east] == ' ') {
                can_move = false;
            }
            break;
        case 's':
            if(playery == height - 1 || dungeon[south][playerx] == '-' || dungeon[south][playerx] == '|' || dungeon[south][playerx] == ' ') {
                can_move = false;
            }
            break;
        case 'w':
            if(playerx == 0 || dungeon[playery][west] == '-' || dungeon[playery][west] == '|' || dungeon[playery][west] == ' ') {
                can_move = false;
            }
            break;
        case 'r':
            if(dungeon[playery][playerx] != '%') {
                can_move = false;
            }
            break;
        default:
            break;
    }

    return can_move;
}

void Dungeon::print_dungeon(char*& screen, int scrnlength, vector<vector<char>> updating_dungeon, int playerx, int playery, bool (&disc_sectors)[9], bool (&disc_tiles)[80*40]) {

    int playersector = return_sector(playerx, playery);
    if(!disc_sectors[playersector] && dungeon[playery][playerx] != '#') {
        disc_sectors[playersector] = true;
    }

    if(!disc_tiles[80 * playery + playerx]) {
        disc_tiles[80 * playery + playerx] = true;
    }

    int sectstartx,sectendx,sectstarty,sectendy;

    if((dungeon[playery][playerx] == '#' || dungeon[playery][playerx] == '+') && (dungeon[playery][playerx+1] == '#' || dungeon[playery][playerx+1] == '+')) {
        disc_tiles[80*playery+playerx+1] = true;
    } 
    if((dungeon[playery][playerx] == '#' || dungeon[playery][playerx] == '+') && (dungeon[playery][playerx-1] == '#' || dungeon[playery][playerx-1] == '+')) {
        disc_tiles[80*playery+playerx-1] = true;
    }
    if((dungeon[playery][playerx] == '#' || dungeon[playery][playerx] == '+') && (dungeon[playery-1][playerx] == '#' || dungeon[playery-1][playerx] == '+')) {
        disc_tiles[80*(playery-1)+playerx] = true;
    }
    if((dungeon[playery][playerx] == '#' || dungeon[playery][playerx] == '+') && (dungeon[playery+1][playerx] == '#' || dungeon[playery+1][playerx] == '+')) {
        disc_tiles[80*(playery+1)+playerx] = true;
    }

    if(playersector % 3 == 0) {
        sectstartx = 5;
        sectstarty = ((playersector / 3) * 9 + 8);
    } else if((playersector - 1) % 3 == 0) {
        sectstartx = 29;
        sectstarty = (((playersector - 1) / 3) * 9 + 8);
    } else if((playersector - 2) % 3 == 0){
        sectstartx = 55;
        sectstarty = (((playersector - 2) / 3) * 9 + 8);
    }

    sectendx = sectstartx + 21;
    sectendy = sectstarty + 6;

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < length; x++) {
            if((sectstartx <= x && x <= sectendx) && (sectstarty <= y && y <= sectendy) && dungeon[y][x] != '#' && dungeon[playery][playerx] != '#') {
                updating_dungeon[y][x] = dungeon[y][x];
            } else {
                if(disc_sectors[return_sector(x, y)] && (dungeon[y][x] != '#' || (disc_tiles[80*y+x] && dungeon[y][x] == '#'))) {
                   disc_tiles[80*y+x] = true;
                }
                if(!disc_tiles[80*y+x]) {
                    updating_dungeon[y][x] = ' ';
                } else if(dungeon[y][x] == '#' && !disc_tiles[80*y+x]) {
                    updating_dungeon[y][x] = ' ';
                } else if(dungeon[y][x] != '|' && dungeon[y][x] != '-' && dungeon[y][x] != '#' && dungeon[y][x] != '+') {
                    updating_dungeon[y][x] = ' ';
                }
                if(disc_tiles[80*y+x] == true && dungeon[y][x] == '%') {
                    updating_dungeon[y][x] = '%';
                } 
            }
            if(y == playery && x == playerx) {
                updating_dungeon[y][x] = '@';
            }
            screen[scrnlength * y + x] = updating_dungeon[y][x];
        }
    }
}

vector<vector<char>> Dungeon::return_dungeon() {
    return dungeon;
}