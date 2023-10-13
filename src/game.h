#ifndef GAME_H
#define GAME_H

#include "./models/Tank.h"
#include <vector>

class Game
{
private:
    Tank player1Tank;
    Tank player2Tank;
    static constexpr int WALL_HEIGHT = 50;
    static constexpr int WALL_WIDTH = 200;
    static constexpr int INITIAL_LIVES = 3;
    static constexpr int SLEEP_DURATION = 50000; // in microseconds

public:
    Game();  // constructor
    ~Game(); // destructor

    void run();                                                                         // Main game loop
    void initialize();                                                                  // Initialization tasks
    void update();                                                                      // Game logic
    void draw();                                                                        // Render game state
    void checkCollisions();                                                             // Check for collisions
    void handleProjectiles(Tank &playerTank);                                           // Handle projectiles
    void displayVictoryMessage(int winnerPlayerColor, const std::string &winnerPlayer); // Victory message
    void drawLayout();                                                                  // Draw the game layout
};

#endif // GAME_H
