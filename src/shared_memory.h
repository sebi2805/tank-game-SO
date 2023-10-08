#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

enum CellType
{
          EMPTY,
          WALL,
          TANK1,
          TANK2,
          PROJECTILE
};

typedef struct
{
          enum CellType **matrix;
          int rows;
          int cols;
} GameBoard;

GameBoard *allocate_shared_memory(int rows, int cols);

#endif // SHARED_MEMORY_H
