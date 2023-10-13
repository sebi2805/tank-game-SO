#include "Game.h" // Make sure to include the Game class header
#include <iostream>

int main()
{
    try
    {
        Game gameInstance;  // Create an instance of the Game class
        gameInstance.run(); // Start the game
    }
    catch (const std::exception &e)
    {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
