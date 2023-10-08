#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Verifică dacă o celulă este un perete
int is_wall(int x, int y);

// Mișcă tancul dacă mișcarea este validă
void move_tank(int *x, int *y, char direction);
void shoot_projectile(int *projX, int *projY, int tankX, int tankY, char lastDirection);
#endif // GAME_LOGIC_H
