#include "game_functions.h"
#include <ncurses.h>  // For ncurses functionality
#include <sys/mman.h> // For shm_open, mmap
#include <fcntl.h>    // For O_* constants
#include <sys/stat.h> // For mode constants
#include <unistd.h>   // For ftruncate
#include <iostream>   // For std::cerr, std::endl
#include "./models/Tank.h"
#include <signal.h>
#include "global_constants.h"

void initializeNcurses()
{
          // Initialize the ncurses screen
          initscr();
          keypad(stdscr, TRUE);  // Enable keypad
          nodelay(stdscr, TRUE); // Makes getch non-blocking
          start_color();         // Start color functionality

          // Initialize color pairs
          init_pair(1, COLOR_BLUE, COLOR_BLACK);    // Wall color
          init_pair(2, COLOR_GREEN, COLOR_BLACK);   // First tank and its projectiles
          init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // Second tank and its projectiles
          init_pair(4, COLOR_RED, COLOR_BLACK);     // Lives

          // Additional settings
          noecho();    // Turn off key echoing
          cbreak();    // Take input chars one at a time, no wait for \n
          curs_set(0); // Hide the cursor

          // Validate ncurses initialization
          if (has_colors() == FALSE)
          {
                    endwin();
                    std::cerr << "Your terminal does not support color\\n";
                    exit(1);
          }
};

void handleCollision(Tank &shooter, Tank &target, Cell *board, int GAME_COLS)
{
          for (size_t i = 0; i < shooter.getProjectiles().size(); /* no increment */)
          {
                    auto &projectile = shooter.getProjectiles()[i];

                    if (target.isHit(projectile->getX(), projectile->getY()))
                    {
                              target.decreaseLife(); // Decrease the life of the tank that got hit

                              // Update board to reflect the decreased life
                              board[4 * GAME_COLS + (target.getX() + target.getLives())].character = ' ';
                              board[4 * GAME_COLS + (target.getX() + target.getLives())].color = 0; // Default color

                              shooter.removeProjectile(i); // Remove the projectile that caused the hit

                              // Redraw the tank at its current position
                              board[target.getX() * GAME_COLS + target.getY()].character = 'T'; // Assuming 'T' represents the tank
                              board[target.getX() * GAME_COLS + target.getY()].color = target.getColor();
                    }
                    else
                    {
                              ++i;
                    }
          }
}

void checkCollisions(Tank &player1Tank, Tank &player2Tank, Cell *board)
{

          handleCollision(player1Tank, player2Tank, board, GAME_COLS);
          handleCollision(player2Tank, player1Tank, board, GAME_COLS);
}

void initializeLayout(Cell *board, Tank &player1Tank, Tank &player2Tank)
{

          // Initialize Walls
          for (int i = 10; i < GAME_ROWS; ++i)
          {
                    for (int j = 0; j < GAME_COLS; ++j)
                    {
                              if (is_wall(i, j))
                              {
                                        board[i * GAME_COLS + j].character = '#';
                                        board[i * GAME_COLS + j].color = 1; // Blue
                              }
                    }
          }

          // Initialize Tanks
          // Assuming Tank class has methods getX(), getY()
          board[player1Tank.getX() * GAME_COLS + player1Tank.getY()].character = 'T';
          board[player1Tank.getX() * GAME_COLS + player1Tank.getY()].color = player1Tank.getColor(); // Orange

          board[player2Tank.getX() * GAME_COLS + player2Tank.getY()].character = 'T';
          board[player2Tank.getX() * GAME_COLS + player2Tank.getY()].color = player2Tank.getColor(); // Magenta/Purple
}

void checkGameOver(Tank &player1Tank, Tank &player2Tank, Cell *board)
{

          if (player1Tank.getLives() <= 0 || player2Tank.getLives() <= 0)
          {
                    // Clear the board
                    for (int i = 0; i < 50 * GAME_COLS; ++i)
                    {
                              board[i].character = ' ';
                              board[i].color = 0; // Default color
                    }

                    // Prepare the game-over message
                    std::string message = "Game Over! ";
                    if (player1Tank.getLives() <= 0)
                    {
                              message += "Player 2 Wins!";
                    }
                    else
                    {
                              message += "Player 1 Wins!";
                    }

                    // Display the game-over message in the middle of the board
                    int start_position = (GAME_COLS / 2) - (message.size() / 2);
                    for (int i = 0; i < message.size(); ++i)
                    {
                              board[25 * GAME_COLS + (start_position + i)].character = message[i];
                              board[25 * GAME_COLS + (start_position + i)].color = 1; // Any color you like for the message
                    }
                    refresh();
                    // Optionally, pause and wait for user input before exiting or resetting
                    int ch = getch();
                    if (ch == 'q')
                    {
                              // Exit the game or reset as you wish
                    }
          }
}

void drawPlayerLives(Tank &playerTank, Cell *board, int position_for_player, int playerColor, const std::string &playerText)
{

          // Clear old lives and text
          for (int i = -20; i < 20; ++i)
          { // Clear a larger area around the lives and text
                    board[4 * GAME_COLS + (position_for_player + i)].character = ' ';
                    board[4 * GAME_COLS + (position_for_player + i)].color = 0; // Default color
          }

          // Write "Player X:" to the board
          int text_position_for_player = position_for_player - playerText.size(); // Adjust based on the length of the player text

          for (int i = 0; i < playerText.size(); ++i)
          {
                    board[4 * GAME_COLS + (text_position_for_player + i)].character = playerText[i];
                    board[4 * GAME_COLS + (text_position_for_player + i)].color = playerColor;
          }

          // Draw new lives
          for (int i = 0; i < playerTank.getLives(); ++i)
          {
                    board[4 * GAME_COLS + (position_for_player + i)].character = '@';
                    board[4 * GAME_COLS + (position_for_player + i)].color = 4; // Red
          }
}

void drawBoard(Cell *board)
{

          // Iterate through the entire game board
          for (int i = 0; i < GAME_ROWS; ++i)
          {
                    for (int j = 0; j < GAME_COLS; ++j)
                    {
                              // Get the Cell from the board
                              Cell cell = board[i * GAME_COLS + j];

                              // Set the color for the cell
                              attron(COLOR_PAIR(cell.color));

                              // Draw the cell character at the appropriate position
                              mvprintw(i, j, "%c", cell.character);

                              // Turn off the color for the cell
                              attroff(COLOR_PAIR(cell.color));
                    }
          }

          // Refresh the screen to show changes
          refresh();
}

void handleProjectiles(Tank &playerTank, Cell *board)
{

          for (size_t i = 0; i < playerTank.getProjectiles().size(); /* no increment */)
          {
                    auto &projectile = playerTank.getProjectiles()[i];

                    // Erase old position from the board
                    board[projectile->getX() * GAME_COLS + projectile->getY()].character = ' ';
                    board[projectile->getX() * GAME_COLS + projectile->getY()].color = 0; // Default color

                    // Move the projectile
                    projectile->move();

                    // Check for wall collision
                    if (is_wall(projectile->getX(), projectile->getY()))
                    {
                              playerTank.removeProjectile(i);
                    }
                    else
                    {
                              // Update new position in the board
                              board[projectile->getX() * GAME_COLS + projectile->getY()].character = '*';
                              board[projectile->getX() * GAME_COLS + projectile->getY()].color = playerTank.getColor();

                              ++i; // Only increment if the projectile wasn't removed
                    }
          }
}
