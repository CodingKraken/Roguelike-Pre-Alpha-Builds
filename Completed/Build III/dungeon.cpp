#include "gameclasses.h"
#include "functions.h"

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

bool Dungeon::is_move_valid(char direction, int px, int py) {

    //define cardinal directions from player coords
    int north = py - 1;
    int east = px + 1;
    int south = py + 1;
    int west = px - 1;

    bool can_move = true;

    switch(direction) {
        //movement checks
        case 'n':
            if(dungeon[north][px] == '-' || dungeon[north][px] == '|' || dungeon[north][px] == ' ') {
                can_move = false;
            }
            break;
        case 'e':
            if(dungeon[py][east] == '-' || dungeon[py][east] == '|' || dungeon[py][east] == ' ') {
                can_move = false;
            }
            break;
        case 's':
            if(dungeon[south][px] == '-' || dungeon[south][px] == '|' || dungeon[south][px] == ' ') {
                can_move = false;
            }
            break;
        case 'w':
            if(dungeon[py][west] == '-' || dungeon[py][west] == '|' || dungeon[py][west] == ' ') {
                can_move = false;
            }
            break;
        //stairs check
        case 'r':
            if(dungeon[py][px] != '%') {
                can_move = false;
            }
            break;
        default:
            can_move = false;
            break;
    }

    return can_move;
}

//WHY DOES IT HAVE SO MANY ARGUMENTS?!?!?!?
void Dungeon::print_dungeon(char*& screen, int scrnlength, vector<vector<char>> updating_dungeon, 
        Player &player, vector<Enemy*> enemyarr, vector<Potion*> potionarr, bool visible) {

    int px = player.playerx;
    int py = player.playery;
    int playersector = return_sector(px, py);

    //check if player is in a previously discovered sector and update accordingly.
    if(!player.disc_sectors[playersector] && dungeon[py][px] != '#') {
        player.disc_sectors[playersector] = true;
    }

    //if on discovered tile, mark as discovered
    if(!player.disc_tiles[80 * py + px]) {
        player.disc_tiles[80 * py + px] = true;
    }

    int sectstartx,sectendx,sectstarty,sectendy;

    char playertile = dungeon[py][px];

    //if player is in a corridor, mark adjacent tiles as discovered.
    if(playertile == '#' || playertile == '+') {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(dungeon[py+i][px+j] == '#' || dungeon[py+i][px+j] == '+') {
                    player.disc_tiles[80*(py+i)+(px+j)] = true;
                }
            }
        }
    }

    //get start/end coords of active player sector
    if(playersector % 3 == 0) {
        sectstartx = 5;
        sectstarty = ((playersector / 3) * 9 + 8);
    } else if((playersector - 1) % 3 == 0) {
        sectstartx = 29;
        sectstarty = (((playersector - 1) / 3) * 9 + 8);
    } else if((playersector - 2) % 3 == 0) {
        sectstartx = 55;
        sectstarty = (((playersector - 2) / 3) * 9 + 8);
    }

    sectendx = sectstartx + 23;
    sectendy = sectstarty + 8;

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < length; x++) {
            if((sectstartx <= x && x <= sectendx) && (sectstarty <= y && y <= sectendy) && dungeon[y][x] != '#' && dungeon[py][px] != '#' || visible) {
                updating_dungeon[y][x] = dungeon[y][x];
                for(int i = 0; i < potionarr.size(); i++) {
                    if(potionarr[i]->xcord == x && potionarr[i]->ycord == y) {
                        updating_dungeon[y][x] = potionarr[i]->letter;
                    }
                }
                for(int j = 0; j < enemyarr.size(); j++) {
                    if(enemyarr[j]->getx() == x && enemyarr[j]->gety() == y)  {
                        updating_dungeon[y][x] = enemyarr[j]->letter;
                    }
                }

            } else {
                if(player.disc_sectors[return_sector(x, y)] && dungeon[y][x] != '#') {
                    player.disc_tiles[80*y+x] = true;
                }
                if(player.disc_tiles[80*y+x]) {
                        updating_dungeon[y][x] = dungeon[y][x];
                } else {
                        updating_dungeon[y][x] = ' ';
                }
                if(dungeon[y][x] != '|' && dungeon[y][x] != '-' && dungeon[y][x] != '#' && dungeon[y][x] != '+') {
                    updating_dungeon[y][x] = ' ';
                }
                if(player.disc_tiles[80*y+x] && dungeon[y][x] == '%') {
                    updating_dungeon[y][x] = '%';
                }
            }
            if(y == py && x == px) {
                updating_dungeon[y][x] = '@';
            }
            screen[scrnlength * y + x] = updating_dungeon[y][x];
        }
    }
}

vector<vector<char>> Dungeon::return_dungeon() {
    return dungeon;
}