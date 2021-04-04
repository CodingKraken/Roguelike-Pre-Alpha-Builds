#include "functions.h"
#include "gameclasses.h"

Potion::Potion(int x, int y, int variant) {
    type = variant;
    switch(variant) {
        case 0:
            value = random(5,26);
            letter = '$';
            break;
        case 1:
            value = random(3, 6);
            letter = '!';
            break;
    }
    xcord = x;
    ycord = y;
}