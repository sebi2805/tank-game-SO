#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

// Definim tipurile de celule din matrice
enum CellType {
    EMPTY,
    WALL,
    TANK1,
    TANK2,
    PROJECTILE
};

// Definim structura matricei
typedef struct {
    enum CellType **matrix;
    int rows;
    int cols;
} GameBoard;

// Functia pentru alocarea memoriei partajate
GameBoard* allocate_shared_memory(int rows, int cols) {
    GameBoard *board = (GameBoard*)malloc(sizeof(GameBoard));
    if (board == NULL) {
        perror("Failed to allocate memory for GameBoard");
        exit(1);
    }

    board->rows = rows;
    board->cols = cols;

    // Alocăm memoria partajată pentru matrice
    board->matrix = mmap(NULL, rows * sizeof(enum CellType*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (board->matrix == MAP_FAILED) {
        perror("Failed to mmap");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        board->matrix[i] = mmap(NULL, cols * sizeof(enum CellType), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (board->matrix[i] == MAP_FAILED) {
            perror("Failed to mmap row");
            exit(1);
        }
    }

    return board;
}
