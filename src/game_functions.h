// game_functions.h
#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "./models/Tank.h"
#include "./shared_memory.h"

void initializeNcurses();
void initializeLayout(Cell *board, Tank &player1Tank, Tank &player2Tank);
void drawBoard(Cell *board);
void handleProjectiles(Tank &playerTank, Cell *board);
void checkCollisions(Tank &player1Tank, Tank &player2Tank, Cell *board);
void drawPlayerLives(Tank &playerTank, Cell *board, int position_for_player, int playerColor, const std::string &playerText);
void checkGameOver(Tank &player1Tank, Tank &player2Tank, Cell *board);
void handle_signal(int signal);
bool is_wall(int x, int y);

#endif // GAME_FUNCTIONS_H
