#include "gameclasses.h"
#include "functions.h"

void Dungeon::init_room(int xpos, int ypos, int& roomlength, int& roomheight, bool is_exitroom, int xoffset, int yoffset) {

    if(return_sector(xpos, ypos + roomheight+1) != return_sector(xpos, ypos) || ypos + roomheight > height) {
        roomheight -= (yoffset+1);
    }

    if((!return_sector(xpos, ypos) - 2) % 3 == 0) {
        for(int i = 0; i < 4; i++) {
            if(return_sector(xpos+roomlength+i, ypos) != return_sector(xpos,ypos)) {
                roomlength -= (xoffset+(7-i));
                break;
            }
        }
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

//This function is *really* bad, and needs a better implementation
void Dungeon::init_corridors(int startsector, int targetsector, bool horizontal, int roominfo[9][4]) {

    int startx,starty,endx,endy;
    int ssectorx,ssectory;
    int tsectorx, tsectory;
    int dx,dy;

    ssectorx = return_roomcord(startsector, true);
    ssectory = return_roomcord(startsector, false);
    
    tsectorx = return_roomcord(targetsector, true);
    tsectory = return_roomcord(targetsector, false);

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

    bool has_shifted = false;
    int midpoint;

    if(horizontal) {

        if(dx > 0) {
            midpoint = random(startx+1+(dx/5), endx-(dx/5));
        } else {
            //note that dx is negative here
            midpoint = random(endx+1-(dx/5), startx+(dx/5));
        }

        //horizontal corridor loop
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

        //dy must be 3 or greater
        midpoint = (starty+1 + endy)/2 + random(-1, 1);

        //vertical corridor generation loop
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

void Dungeon::init_dungeon(int& px, int& py, vector<int>& roomsec) {

    int roominfo[9][4];
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

        int randxmin = 0;
        if(roomsector != 0 && roomsector != 3 && roomsector != 6) {
            if(sectors[roomsector - 1] == 1) {
                int sum = ranxpos - (roominfo[roomsector-1][0]+roominfo[roomsector-1][2]);
                if(sum < 4 && sum > 0) {
                    randxmin = 2+ranxpos - (roominfo[roomsector-1][0]+roominfo[roomsector-1][2]);
                }
            }
        }

        int randxoffset = random(randxmin,7);
        int randyoffset = random(0,2);

        ranxpos += randxoffset;
        ranypos += randyoffset;

        if(i == 0) {
            px = ranxpos + random(2,8);
            py = ranypos + 2;
        }

        sectors[roomsector] = 1;

        ranlength = random(12,22-randxoffset);
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

    int start = random(0, limit);
    int treepos = 0;

    vector<vector<int>> roomconnections (9);
    vector<int> roomtree;
    roomtree.push_back(roomsec[start]);
    bool finished = false;
    bool cant_connect;
    while(!finished) {
        for(int i = 0; i < limit; i++) {
            bool horizontal;
            bool possible = true;
            cant_connect = true;
            
            for(int j = 0; j < roomtree.size(); j++) {
                if(roomsec[i] == roomtree[j]) {
                    possible = false;
                    break;
                }
            }
            if(possible) {
                if(can_connect(roomtree[treepos], roomsec[i], sectors, horizontal)) {
                    init_corridors(roomtree[treepos], roomsec[i], horizontal, roominfo);
                    roomtree.push_back(roomsec[i]);
                    roomconnections[roomtree[treepos]].push_back(roomsec[i]);
                    roomconnections[roomsec[i]].push_back(roomtree[treepos]);

                    treepos = roomtree.size() - 1;
                    cant_connect = false;
                    break;
                } else {
                    possible = false;
                }
            }
        }
        if(cant_connect) {
            treepos--;
        }

        if(roomtree.size() == roomsec.size() && (treepos + 1 == 0 || treepos == roomsec.size())) {
            finished = true;
            break;
        }
    }
    
    int extracorridors = random(1, 4);
    bool horizontal;
    for(int i = 0; i < extracorridors; i++) {
        for(int j = 0; j < roomsec.size(); j++) {
            if(roomconnections[roomsec[j]].size() < 2) {
                for(int k = 0; k < roomsec.size(); k++) {
                    if(roomconnections[roomsec[j]][0] != roomsec[k] && can_connect(roomsec[j], roomsec[k], sectors, horizontal)) {
                        init_corridors(roomsec[j], roomsec[k], horizontal, roominfo);
                        roomconnections[roomsec[j]].push_back(roomsec[k]);
                        roomconnections[roomsec[k]].push_back(roomsec[j]);
                        j = roomsec.size();
                        break;
                    }
                }
            }
        }
    }
}