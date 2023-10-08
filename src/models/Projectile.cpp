#include "Projectile.h"
Projectile::Projectile(int tankId, int x, int y, char direction, int color)
    : tankId(tankId), x(x), y(y), direction(direction), color(color) {}
void Projectile::move()
{
    switch (direction)
    {
    case 'w':
        --x;
        break;
    case 's':
        ++x;
        break;
    case 'a':
        --y;
        break;
    case 'd':
        ++y;
        break;
    }
}