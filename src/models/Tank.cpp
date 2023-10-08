#include "Tank.h"
#include <stdexcept>
#include <ncurses.h>

int Tank::instanceCount = 0;

Tank::Tank(int id, int x, int y, int color)
    : id(id), color(color), x(x), y(y), lastDirection('w')
{
          if (++instanceCount > 2)
          {
                    throw std::runtime_error("Cannot create more than two tanks.");
          }
}

bool is_wall(int x, int y)
{
          return (x == 0 || x == 49 || y == 0 || y == 199);
}

void Tank::move(int key)
{
          int newX = x, newY = y;

          if (id == 1)
          {
                    if (key == 'w')
                    {
                              newX = x - 1;
                    }
                    else if (key == 'a')
                    {
                              newY = y - 1;
                    }
                    else if (key == 's')
                    {
                              newX = x + 1;
                    }
                    else if (key == 'd')
                    {
                              newY = y + 1;
                    }
          }
          else if (id == 2)
          {
                    if (key == KEY_UP)
                    {
                              newX = x - 1;
                    }
                    else if (key == KEY_LEFT)
                    {
                              newY = y - 1;
                    }
                    else if (key == KEY_DOWN)
                    {
                              newX = x + 1;
                    }
                    else if (key == KEY_RIGHT)
                    {
                              newY = y + 1;
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
