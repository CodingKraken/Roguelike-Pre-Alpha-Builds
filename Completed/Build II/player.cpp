#include "classes.h"

Player::Player() {

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

void Player::set_cords(int x, int y) {
    playerx = x;
    playery = y;
}