#include "Tank.h"
#include <stdexcept>
#include <ncurses.h>

int Tank::instanceCount = 0;

bool Tank::isHit(int x, int y)
{
    if (x == this->x && y == this->y)
        return true;
    return false;
}
Tank::Tank(int id, int x, int y, int color, int initialLives) : id(id), x(x), y(y), color(color), lives(initialLives)
{
    if (++instanceCount > 2)
    {
        throw std::runtime_error("Cannot create more than two tanks.");
    }
}
void Tank::decreaseLife()
{
    --lives;
}

int Tank::getLives() const
{
    return lives;
}
bool Tank::checkCollision(const std::vector<Projectile *> &projectiles)
{
    for (const auto &projectile : projectiles)
    {
        if (x == projectile->x && y == projectile->y)
        {
            return true;
        }
    }
    return false;
}

bool is_wall(int x, int y)
{
    return (x == 10 || x == 49 || y == 0 || y == 199);
}

void Tank::move(int key)
{

    int newX = x, newY = y;

    if (id == 1)
    {
        if (key == 'w')
        {
            newX = x - 1;
            lastDirection = 'u';
        }
        else if (key == 'a')
        {
            newY = y - 1;
            lastDirection = 'l';
        }
        else if (key == 's')
        {
            newX = x + 1;
            lastDirection = 'd';
        }
        else if (key == 'd')
        {
            newY = y + 1;
            lastDirection = 'r';
        }
    }
    else if (id == 2)
    {
        if (key == KEY_UP)
        {
            newX = x - 1;
            lastDirection = 'u';
        }
        else if (key == KEY_LEFT)
        {
            newY = y - 1;
            lastDirection = 'l';
        }
        else if (key == KEY_DOWN)
        {
            newX = x + 1;
            lastDirection = 'd';
        }
        else if (key == KEY_RIGHT)
        {
            newY = y + 1;
            lastDirection = 'r';
        }
    }

    if (!is_wall(newX, newY))
    {
        x = newX;
        y = newY;
    }
}

void Tank::shoot()
{
    projectiles.push_back(std::make_unique<Projectile>(id, x, y, lastDirection, color)); // Change to make_unique
}

void Tank::removeProjectile(int index)
{
    if (index >= 0 && index < projectiles.size())
    {
        projectiles.erase(projectiles.begin() + index); // No need to delete, unique_ptr takes care of it
    }
}
