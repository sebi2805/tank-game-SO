#ifndef TANK_MODEL_H
#define TANK_MODEL_H
#include "./Projectile.h"
#include "../shared_memory.h"
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

          Tank(int id, int x, int y, int color, int initialLives);

          // Getter functions
          int getColor() const;
          int getId() const;
          int getX() const;
          int getY() const;
          char getLastDirection() const;
          int getLives() const;
          const std::vector<std::unique_ptr<Projectile>> &getProjectiles() const;

          // Setter functions
          void setColor(int color);
          void setId(int id);
          void setX(int x);
          void setY(int y);
          void setLastDirection(char lastDirection);
          void setLives(int lives);

          void decreaseLife();
          void move(int key, Cell *board);
          void shoot();
          void removeProjectile(int index);
          bool isHit(int x, int y);

private:
          int color;
          int id;
          int x, y;
          char lastDirection;
          int lives;
          std::vector<std::unique_ptr<Projectile>> projectiles;
};

bool is_wall(int x, int y);

struct SharedData
{
          Cell board[50 * 200]; // Your existing 2D board
          Tank player1Tank;
          Tank player2Tank;
          int processCounter; // New field to count the number of attached processes
};
#endif