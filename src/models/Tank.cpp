#include "Tank.h"
#include <stdexcept>
#include <ncurses.h>
#include "../global_constants.h"

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
bool is_wall(int x, int y)
{
    return (x == 10 || x == 49 || y == 0 || y == 199);
}
void Tank::decreaseLife()
{
    --lives;
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

    // Remove the tank's old position from the board
    board[x * GAME_COLS + y].character = ' ';
    board[x * GAME_COLS + y].color = 0; // Default color

    if (!is_wall(newX, newY))
    {
        x = newX;
        y = newY;
    }

    // Update the tank's new position in the board
    board[x * GAME_COLS + y].character = 'T';
    board[x * GAME_COLS + y].color = color;
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

// Getter functions
int Tank::getColor() const { return color; }
int Tank::getId() const { return id; }
int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
char Tank::getLastDirection() const { return lastDirection; }
int Tank::getLives() const { return lives; }
const std::vector<std::unique_ptr<Projectile>> &Tank::getProjectiles() const
{
    return projectiles;
}

// Setter functions
void Tank::setColor(int newColor) { color = newColor; }
void Tank::setId(int newId) { id = newId; }
void Tank::setX(int newX) { x = newX; }
void Tank::setY(int newY) { y = newY; }
void Tank::setLastDirection(char newLastDirection) { lastDirection = newLastDirection; }
void Tank::setLives(int newLives) { lives = newLives; }