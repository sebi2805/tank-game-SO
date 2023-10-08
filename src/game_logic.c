#include "game_logic.h"

// Verifică dacă o celulă este un perete
int is_wall(int x, int y) {
    return (x == 0 || x == 49 || y == 0 || y == 199);
}

// Mișcă tancul dacă mișcarea este validă
void move_tank(int *x, int *y, char direction) {
    int newX = *x;
    int newY = *y;

    switch (direction) {
        case 'w':
            --newX;
            break;
        case 's':
            ++newX;
            break;
        case 'a':
            --newY;
            break;
        case 'd':
            ++newY;
            break;
    }

    if (!is_wall(newX, newY)) {
        *x = newX;
        *y = newY;
    }
}
void shoot_projectile(int *projX, int *projY, int tankX, int tankY, char lastDirection) {
    *projX = tankX;
    *projY = tankY;

    switch (lastDirection) {
        case 'w':
            --(*projX);
            break;
        case 's':
            ++(*projX);
            break;
        case 'a':
            --(*projY);
            break;
        case 'd':
            ++(*projY);
            break;
    }
}