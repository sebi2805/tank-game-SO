#include "Tank.h"
#include <stdexcept>
#include <ncurses.h>

int Tank::instanceCount = 0;

Tank::Tank(int id, int x, int y, int color) : id(id), x(x), y(y), color(color), lastDirection('w')
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
          if (id == 1)
          {
                    if (key == 'w')
                    {
                              // move up
                    }
                    else if (key == 'a')
                    {
                              // move left
                    }
                    else if (key == 's')
                    {
                              // move down
                    }
                    else if (key == 'd')
                    {
                              // move right
                    }
          }
          else if (id == 2)
          {
                    if (key == KEY_UP)
                    {
                              // move up
                    }
                    else if (key == KEY_LEFT)
                    {
                              // move left
                    }
                    else if (key == KEY_DOWN)
                    {
                              // move down
                    }
                    else if (key == KEY_RIGHT)
                    {
                              // move right
                    }
          }
}

void Tank::shoot()
{
          projectiles.push_back(new Projectile(id, x, y, lastDirection, color));
}
void Tank::removeProjectile(int index)
{
          if (index >= 0 && index < projectiles.size())
          {
                    delete projectiles[index];
                    projectiles.erase(projectiles.begin() + index);
          }
}