#include "gameclasses.h"
#include "functions.h"

Player::Player() {
    health = maxhealth;
};

void Player::move(char direction, Dungeon dungeon) {
    switch(direction) {
        case 'n':
            if(dungeon.is_move_valid('n', playerx, playery)) {
                playery--;
            }
            break;
        case 'e':
            if(dungeon.is_move_valid('e', playerx, playery)) {
                playerx++;
            }
            break;
        case 's':
            if(dungeon.is_move_valid('s', playerx, playery)) {
                playery++;
            }
            break;
        case 'w':
            if(dungeon.is_move_valid('w', playerx, playery)) {
                playerx--;
            }
    }
}

void Player::heal(int amount) {
    health += amount;
    if(health > maxhealth) {
        health = maxhealth;
    }
}

void Player::set_cords(int x, int y) {
    playerx = x;
    playery = y;
}

void Player::set_name(string name) {
    playername = name;
}

void Player::reset_map() {
    for(int i = 0; i < 9; i++) {
        disc_sectors[i] = false;
    }
    for(int j = 0; j < 80 * 40; j++) {
        disc_tiles[j] = false;
    }
}

void Player::reset_stats() {
    health = 10;
    level = 1;
    maxhealth = 10;
    defense = 1;
    exp = 0;
    expperlvl = 4;
    gold = 0;
}

void Player::attack(int damage) {
    damage *= (10.0f/(10.0f+defense));
    if(damage <= 1) {
        damage = 0;
        if(random(0,2) == 1) {
            damage = 1;
        }
    }
    health -= damage;
}

void Player::levelup() {
    level++;
    maxhealth = 2*(level-1)+10;
    defense = 1.5f*(level - 1)+1;
    expperlvl = 2*(level-1) + 4;
}