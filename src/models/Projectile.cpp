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
void Projectile::draw()
{
    attron(COLOR_PAIR(this->color));
    mvprintw(this->x, this->y, "*");
    attroff(COLOR_PAIR(this->color));
}