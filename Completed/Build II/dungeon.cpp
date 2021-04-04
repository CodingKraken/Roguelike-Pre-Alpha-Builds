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

void Dungeon::init_room(int xpos, int ypos, int& roomlength, int& roomheight, bool is_exitroom, int xoffset, int yoffset) {

    if(return_sector(xpos, ypos + roomheight+1) != return_sector(xpos, ypos) || ypos + roomheight > height) {
        roomheight -= yoffset;
        roomheight--;
    }

    if(return_sector(xpos + roomlength+3,ypos) != return_sector(xpos,ypos) || xpos + roomlength > length) {
        roomlength -= xoffset;
        roomlength -= 3;
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

void Dungeon::init_corridors(int startsector, vector<int>& connections, vector<vector<int>>& roomcons, int sectors[9], int xoffset, int yoffset, int roominfo[9][4]) {

    bool horizontal;
    int targetsector = 0;
    while(have_connected(roomcons, startsector, targetsector) || !can_connect(startsector, targetsector, sectors, horizontal)) {
        targetsector++;
        if(targetsector == 9) {
            return;
        }
    }

    connections[startsector] = targetsector;
    connections[targetsector] = startsector;
    roomcons[startsector].push_back(targetsector);
    roomcons[targetsector].push_back(startsector);

    int startx,starty,endx,endy;
    int ssectorx,ssectory;
    int tsectorx, tsectory;
    int dx,dy;

    if(startsector % 3 == 0) {
        ssectorx = 0;
        ssectory = startsector / 3;
    } else if((startsector - 1) % 3 == 0) {
        ssectorx = 1;
        ssectory = (startsector - 1) / 3;
    } else if((startsector - 2) % 3 == 0){
        ssectorx = 2;
        ssectory = (startsector - 2) / 3;
    }

    if(targetsector % 3 == 0) {
        tsectorx = 0;
        tsectory = targetsector / 3;
    } else if((targetsector - 1) % 3 == 0) {
        tsectorx = 1;
        tsectory = (targetsector - 1) / 3;
    } else if((targetsector - 2) % 3 == 0){
        tsectorx = 2;
        tsectory = (targetsector - 2) / 3;
    }

    int sroomlength = roominfo[startsector][0];
    int sroomheight = roominfo[startsector][1];
    int sroomx = roominfo[startsector][2];
    int sroomy = roominfo[startsector][3];

    int troomlength = roominfo[targetsector][0];
    int troomheight = roominfo[targetsector][1];
    int troomx = roominfo[targetsector][2];
    int troomy = roominfo[targetsector][3];

    if(ssectorx > tsectorx && ssectory == tsectory) {
        startx = sroomx;
        starty = random(sroomy+1,sroomy+sroomheight-1);
        endx = troomx+troomlength-1;
        endy = random(troomy+1, troomy+troomheight-1);
        while(starty == endy) {
            endy = random(troomy+1, troomy+troomheight-1);
        }
    } else if(ssectorx < tsectorx && ssectory == tsectory) {
        startx = sroomx+sroomlength-1;
        starty = random(sroomy+1,sroomy+sroomheight-1);
        endx = troomx;
        endy = random(troomy+1, troomy+troomheight-1);
        while(starty == endy) {
            endy = random(troomy+1, troomy+troomheight-1);
        }
    } else if(ssectory > tsectory && ssectorx == tsectorx) {
        startx = random(sroomx+1, sroomlength+sroomx-1);
        starty = sroomy;
        endx = random(troomx+1, troomlength+troomx-1);
        endy = troomy+troomheight;
        while(startx == endx) {
            endx = random(troomx+1, troomlength+troomx-1);
        }
    } else if(ssectory < tsectory && ssectorx == tsectorx) {
        startx = random(sroomx+1, sroomlength+sroomx-1);
        starty = sroomy+sroomheight-1;
        endx = random(troomx+1, troomlength+troomx-1);
        endy = troomy;
        while(startx == endx) {
            endx = random(troomx+1, troomlength+troomx-1);
        }
    }

    dx = endx - startx;
    dy = endy - starty;

    int xrate,yrate;

    bool has_shifted = false;

    if(horizontal) {
        int midpoint = (startx + endx)/2 + random(-1, 1);
        for(int x = startx; x != endx+(dx/abs(dx)); x += dx / abs(dx)) {
            if(x == midpoint && !has_shifted) {
                for(int y = starty; y != endy; y += dy/abs(dy)) {
                    dungeon[y][x] = '#';
                }
                starty = endy;
                has_shifted = true;
            }
            if(dungeon[starty][x] == '|') {
                dungeon[starty][x] = '+';
            } else if(dungeon[starty][x-1] == '|' && x == endx + 1 && has_shifted) {
                dungeon[starty][x-1] = '+';
                dungeon[starty][x] = '#';
            } else if(dungeon[starty][x+1] == '|' && x == endx - 1 && has_shifted) {
                dungeon[starty][x+1] = '+';
                dungeon[starty][x] = '#';
            } else if(dungeon[starty][x] == ' ') {
                dungeon[starty][x] = '#';
            }
        }
    } else {
        int endcon = endy +(dy/abs(dy));
        int midpoint = (starty+1 + endy)/2 + random(-1, 1);
        for(int y = starty; y != endcon; y += dy / abs(dy)) {
            if(y == midpoint && !has_shifted) {
                for(int x = startx; x != endx; x += dx/abs(dx)) {
                    dungeon[y][x] = '#';
                }
                startx = endx;
                has_shifted = true;
            }
            if(dungeon[y][startx] == '-') {
                dungeon[y][startx] = '+';  
            } else if(dungeon[y-1][startx] == '-' && y == endcon + 1 && has_shifted) {
                dungeon[y][startx] = '#';
                dungeon[y-1][startx] = '+';
            } else if(dungeon[y+1][startx] == '-' && y == endcon - 1 && has_shifted) {
                dungeon[y][startx] = '#';
                dungeon[y+1][startx] = '+';
            } else if(dungeon[y][startx] == ' ') {
                dungeon[y][startx] = '#';
            }
        }
    }
}

void Dungeon::init_dungeon(int& playerx, int& playery) {

    int roominfo[9][4];
    vector<int> roomsec;
    for(int i = 0; i < height; i++) {
        dungeon[i].resize(length);
        for(int j = 0; j < length; j++) {
            dungeon[i][j] = ' ';
        }
    }

    int limit = random(7,9);
    int ranxpos,ranypos,ranlength,ranheight;
    int sectors[9] = {0};

    int randexit = random(1, limit);

    for(int i = 0; i < limit; i++) {
        int roomsector = random(0,9);
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
            roomsector = random(0,9);
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

        int randxoffset = random(0,6);
        int randyoffset = random(0,2);
        ranxpos += randxoffset;
        ranypos += randyoffset;

        if(i == 0) {
            playerx = ranxpos + random(2,8);
            playery = ranypos + 2;
        }

        sectors[roomsector] = 1;

        ranlength = random(12,22);
        ranheight = random(6, 8);

        if(i == randexit) {
            init_room(ranxpos,ranypos,ranlength,ranheight, true, randxoffset, randyoffset);
        } else {
            init_room(ranxpos,ranypos,ranlength,ranheight, false, randxoffset, randyoffset);
        }
        roomsec.push_back(roomsector);
        roominfo[roomsector][0] = ranlength;
        roominfo[roomsector][1] = ranheight;
        roominfo[roomsector][2] = ranxpos;
        roominfo[roomsector][3] = ranypos;
    }

    int randx = random(0, 9);
    int randy = random(0, 4);

    int start;
    vector<int> hasconnections(9,9);
    vector<int> hasdrawnfrom (9,0);
    vector<vector<int>> roomconnections;
    roomconnections.resize(9,vector<int>(9,9));

    for(int j = 0; j < limit; j++) {
        start = roomsec[j];
        init_corridors(start, hasconnections, roomconnections, sectors, randx, randy, roominfo);
    }
    for(int k = 0; k < roomsec.size(); k++) {
        int cons = roomconnections[roomsec[k]].size();
        for(int h = 0; h < roomconnections[roomsec[k]].size(); h++) {
            if(roomconnections[roomsec[k]][h] == 9) {
                cons--;
            }
        }
        if(cons <= 5 || hasconnections[k] == 9 || hasconnections[hasconnections[k]] == k) {
            init_corridors(roomsec[k], hasconnections, roomconnections, sectors, randx, randy, roominfo);
        }
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

void Dungeon::print_dungeon(char*& screen, int scrnlength, vector<vector<char>> updating_dungeon, int playerx, int playery, bool (&disc_sectors)[9], bool (&disc_tiles)[80*40], bool visible) {

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

    sectendx = sectstartx + 23;
    sectendy = sectstarty + 8;

    bool hide = visible;

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < length; x++) {
            if((sectstartx <= x && x <= sectendx) && (sectstarty <= y && y <= sectendy) && dungeon[y][x] != '#' && dungeon[playery][playerx] != '#') {
                updating_dungeon[y][x] = dungeon[y][x];
            } else {
                if(!hide) {
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