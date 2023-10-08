#ifndef PROJECTILE_MODEL_H
#define PROJECTILE_MODEL_H

class Projectile
{
public:
          int tankId;
          int color;
          int x, y;
          char direction;
          Projectile(int tankId, int x, int y, char direction, int color);
          void move();
};
#endif