#include "Projectile.h"
#include <ncurses.h>
Projectile::Projectile(int tankId, int x, int y, char direction, int color)
    : tankId(tankId), x(x), y(y), direction(direction), color(color) {}
void Projectile::move()
{
    switch (direction)
    {
    case 'u':
        --x;
        break;
    case 'd':
        ++x;
        break;
    case 'l':
        --y;
        break;
    case 'r':
        ++y;
        break;
    }
}
// Getter functions
int Projectile::getTankId() const { return tankId; }
int Projectile::getColor() const { return color; }
int Projectile::getX() const { return x; }
int Projectile::getY() const { return y; }
char Projectile::getDirection() const { return direction; }

// Setter functions
void Projectile::setTankId(int newTankId) { tankId = newTankId; }
void Projectile::setColor(int newColor) { color = newColor; }
void Projectile::setX(int newX) { x = newX; }
void Projectile::setY(int newY) { y = newY; }
void Projectile::setDirection(char newDirection) { direction = newDirection; }