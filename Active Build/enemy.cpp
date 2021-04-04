#include "gameclasses.h"
#include "functions.h"

Enemy::Enemy(int enemyid, int xpos, int ypos) {
    switch(enemyid) {
        case 0:
            letter = 'B';
            health = 3;
            defense = 0;
            damage = 1;
            break;
        case 1:
            letter = 'G';
            health = 6;
            defense = 1;
            damage = 4;
            break;
        case 2:
            letter = 'S';
            health = 5;
            defense = 1;
            damage = 3;
            break;
    }

    xcord = xpos;
    ycord = ypos;
    id = enemyid;
}

Enemy::~Enemy() {
    
};

int Enemy::getx() {
    return xcord;
}

int Enemy::gety() {
    return ycord;
}

int Enemy::get_damage() {
    return damage;
}

void Enemy::attack(int damage) {
    damage = damage * (10.0f/(10.0f+defense));
    health -= damage;
}

bool Enemy::has_died() {
    if(health <= 0) {
        return true;
    }
    return false;
}

bool Enemy::can_attack(Player player) {
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            if(xcord+j == player.playerx && ycord+i == player.playery) {
                return true;
            }
        }
    }
    return false;
}

void Enemy::move(Dungeon dungeon, Player player, vector<Enemy*> enarr) {
    
    int xdir = random(-1,2);
    int ydir = random(-1,2);

    int playerx = player.playerx;
    int playery = player.playery;

    int playersector = return_sector(playerx, playery);
    int enemysector = return_sector(xcord, ycord);

    int distance = dist(playerx, playery, xcord, ycord);

    vector<vector<char>> map = dungeon.return_dungeon();


    switch(id) {
        case 0:
            while(map[ycord+ydir][xcord+xdir] != '.' || is_enemy(enarr, player, (xcord+xdir), (ycord+ydir))) {
                xdir = random(-1,2);
                ydir = random(-1,2);
            }
            break;

        default:
            if(playersector != enemysector || (map[playery][playerx] != map[ycord][xcord]) || distance > 4) {
                while(map[ycord+ydir][xcord+xdir] != '.' || is_enemy(enarr, player, (xcord+xdir), (ycord+ydir))) {
                    xdir = random(-1,2);
                    ydir = random(-1,2);
                }
            } else {
                int dx = xcord - playerx;
                int dy = ycord - playery;

                if(abs(dx) >= abs(dy)) {
                    if(dx > 0 && is_enemy(enarr, player, (xcord-1), ycord) != true) {
                        xdir = -1;
                    } else if(dx < 0 && is_enemy(enarr, player, (xcord+1), ycord) != true) {
                        xdir = 1;
                    } else {
                        xdir = 0;
                    }
                    ydir = 0;
                } else {
                    if(dy > 0 && is_enemy(enarr, player, xcord, (ycord-1)) != true) {
                        ydir = -1;
                    } else if(dy < 0 && is_enemy(enarr, player, xcord, (ycord+1)) != true) {
                        ydir = 1;
                    } else {
                        ydir = 0;
                    }
                    xdir = 0;
                }
            }
        break;
    }

    if(map[ycord+ydir][xcord+xdir] == '|' || map[ycord+ydir][xcord+xdir] == '-' || map[ycord+ydir][xcord+xdir] == ' ') {
        xdir = 0;
        ydir = 0;
    }

    xcord = xcord + xdir;
    ycord = ycord + ydir;
}