#ifndef TANK_MODEL_H
#define TANK_MODEL_H
#include "./Projectile.h"
#include <vector>

class Tank
{
public:
          static int instanceCount;
          int color;
          int id;
          int x, y;
          char lastDirection;
          Tank(int id, int x, int y, int color);
          void move(int direction);
          std::vector<Projectile *> projectiles;
          void shoot();
          void removeProjectile(int index);
};
bool is_wall(int x, int y);
#endif