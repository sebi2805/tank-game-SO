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
Tank::Tank(int id, int x, int y, int color, int initialLives) : id(id), color(color), x(x), y(y), lives(initialLives)
{
    if (++instanceCount > 2)
    {
        throw std::runtime_error("Cannot create more than two tanks.");
    }
    lastDirection = 'u';
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

void Tank::move(int key, Cell *board)
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

    const int cols = 200; // Number of columns in the board

    // Remove the tank's old position from the board
    board[x * cols + y].character = ' ';
    board[x * cols + y].color = 0; // Default color

    if (!is_wall(newX, newY))
    {
        x = newX;
        y = newY;
    }

    // Update the tank's new position in the board
    board[x * cols + y].character = 'T';
    board[x * cols + y].color = color;
}

void Tank::shoot()
{
    int projectileX = x + (lastDirection == 'u' ? -1 : lastDirection == 'd' ? 1
                                                                            : 0);
    int projectileY = y + (lastDirection == 'l' ? -1 : lastDirection == 'r' ? 1
                                                                            : 0);

    projectiles.push_back(std::make_unique<Projectile>(id, projectileX, projectileY, lastDirection, color));
}
void Tank::removeProjectile(int index)
{
    if (index < projectiles.size())
    {
        projectiles.erase(projectiles.begin() + index); // No need to delete, unique_ptr takes care of it
    }
}
void Tank::draw()
{
    attron(COLOR_PAIR(this->color));
    mvprintw(this->x, this->y, "T");
    attroff(COLOR_PAIR(this->color));
}