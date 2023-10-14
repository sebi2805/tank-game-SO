#ifndef TANK_MODEL_H
#define TANK_MODEL_H
#include "./Projectile.h"
#include <vector>
#include <memory>
struct Cell
{
          char character;
          int color;
};
class Tank
{
public:
          static int instanceCount;
          int color;
          int id;
          int x, y;
          char lastDirection;
          int lives;
          Tank(int id, int x, int y, int color, int initialLives);
          void decreaseLife();
          int getLives() const;
          void move(int key, Cell *board);
          std::vector<std::unique_ptr<Projectile>> projectiles;
          void shoot();
          void removeProjectile(int index);
          bool checkCollision(const std::vector<Projectile *> &projectiles);
          bool isHit(int x, int y);
          void draw();
};
bool is_wall(int x, int y);
#endif