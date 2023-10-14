#include <ncurses.h>  // For ncurses functionality
#include <sys/mman.h> // For shm_open, mmap
#include <fcntl.h>    // For O_* constants
#include <sys/stat.h> // For mode constants
#include <unistd.h>   // For ftruncate
#include <iostream>   // For std::cerr, std::endl
#include "./models/Tank.h"
#include <signal.h>
#include "./game_functions.h"

SharedData *sharedData;
int shm_fd; // Also make this global
void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        sharedData->processCounter--;
        if (sharedData->processCounter == 0)
        {
            shm_unlink("GameBoard");
        }
        munmap(sharedData, sizeof(SharedData));
        close(shm_fd);
        endwin();
        exit(1);
    }
}
int main()
{
    // Create or open a shared memory object
    signal(SIGINT, handle_signal);

    shm_fd = shm_open("GameBoard", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    sharedData = static_cast<SharedData *>(mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    Cell *board = sharedData->board;

    if (sharedData->processCounter >= 2)
    {
        std::cerr << "Only two processes can play the game at a time." << std::endl;
        exit(1);
    }
    sharedData->processCounter++;

    if (sharedData->processCounter == 1)
    { // first process
        sharedData->player1Tank = Tank(1, 25, 100, 2, 3);
        sharedData->player2Tank = Tank(2, 25, 150, 3, 3);
    }
    Tank &player1Tank = sharedData->player1Tank;
    Tank &player2Tank = sharedData->player2Tank;
    initializeNcurses();

    while (true)
    {

        int ch = getch(); // Get the user input
        flushinp();       // Flush the input buffer
        initializeLayout(board, player1Tank, player2Tank);

        // Handle shooting and movement
        if (ch == 10)
        { // Enter key for player 2
            player2Tank.shoot();
        }
        else if (ch == 32)
        { // Space key for player 1
            player1Tank.shoot();
        }
        else if (ch != ERR)
        { // Any other key for movement
            player1Tank.move(ch, board);
            player2Tank.move(ch, board);
        }

        // Update the positions of projectiles
        handleProjectiles(player1Tank, board);
        handleProjectiles(player2Tank, board);

        // Check for collisions
        checkCollisions(player1Tank, player2Tank, board);
        checkGameOver(player1Tank, player2Tank, board);
        int halfScreenWidth = 150 / 2;

        // // Calculate starting positions for centered text
        int position_for_player1 = (halfScreenWidth / 2) - (9 + player1Tank.getLives()) / 2;
        int position_for_player2 = halfScreenWidth + (halfScreenWidth / 2) - (9 + player2Tank.getLives()) / 2;
        drawPlayerLives(player1Tank, board, position_for_player1, player1Tank.getColor(), "Player 1: ");
        drawPlayerLives(player2Tank, board, position_for_player2, player2Tank.getColor(), "Player 2: ");

        // Draw the updated board
        drawBoard(board);

        // Add a delay to make it more CPU-friendly
        usleep(50000); // 50 ms delay

        // Break condition, e.g., a keypress to exit
        if (ch == 'q')
        {
            break;
        }
    }
    // Cleanup: Unmap and close the shared memory
    sharedData->processCounter--;
    if (sharedData->processCounter == 0)
    {
        shm_unlink("GameBoard");
    }
    munmap(sharedData, sizeof(SharedData));
    close(shm_fd);

    endwin();
    return 0;
}
