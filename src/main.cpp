#include <fcntl.h>    // For O_* constants
#include <sys/mman.h> // For shm_open, mmap
#include <sys/stat.h> // For mode constants
#include <unistd.h>   // For ftruncate
#include <iostream>   // For std::cerr, std::endl
#include <ncurses.h>  // For ncurses functionality
#include "./models/Tank.h"
#include <signal.h>
struct SharedData
{
    Cell board[50 * 200]; // Your existing 2D board
    Tank player1Tank;
    Tank player2Tank;
    int processCounter; // New field to count the number of attached processes
};
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

void checkCollisions(Tank &player1Tank, Tank &player2Tank, Cell *board)
{
    const int cols = 200; // Number of columns in the board
    const int rows = 50;  // Number of rows in the board
    // Check if any projectile from player1Tank hit player2Tank
    for (size_t i = 0; i < player1Tank.projectiles.size(); /* no increment */)
    {
        auto &projectile = player1Tank.projectiles[i];

        if (player2Tank.isHit(projectile->x, projectile->y))
        {
            player2Tank.decreaseLife(); // Decrease the life of the tank that got hit

            // Update board to reflect the decreased life
            board[4 * cols + (player2Tank.x + player2Tank.getLives())].character = ' ';
            board[4 * cols + (player2Tank.x + player2Tank.getLives())].color = 0; // Default color

            player1Tank.removeProjectile(i); // Remove the projectile that caused the hit

            // Redraw the tank at its current position
            board[player2Tank.x * cols + player2Tank.y].character = 'T';
            board[player2Tank.x * cols + player2Tank.y].color = player2Tank.color;
        }
        else
        {
            ++i;
        }
    }

    // Check if any projectile from player2Tank hit player1Tank
    for (size_t i = 0; i < player2Tank.projectiles.size(); /* no increment */)
    {
        auto &projectile = player2Tank.projectiles[i];

        if (player1Tank.isHit(projectile->x, projectile->y))
        {
            player1Tank.decreaseLife(); // Decrease the life of the tank that got hit

            // Update board to reflect the decreased life
            board[4 * cols + (player1Tank.x + player1Tank.getLives())].character = ' ';
            board[4 * cols + (player1Tank.x + player1Tank.getLives())].color = 0; // Default color

            player2Tank.removeProjectile(i); // Remove the projectile that caused the hit

            // Redraw the tank at its current position
            board[player2Tank.x * cols + player2Tank.y].character = 'T';
            board[player2Tank.x * cols + player2Tank.y].color = player2Tank.color;
        }
        else
        {
            ++i;
        }
    }
};

void initializeLayout(Cell *board, Tank &player1Tank, Tank &player2Tank)
{
    const int rows = 50;
    const int cols = 200;

    // Initialize Walls
    for (int i = 10; i < 50; ++i)
    {
        for (int j = 0; j < 200; ++j)
        {
            if (is_wall(i, j))
            {
                board[i * cols + j].character = '#';
                board[i * cols + j].color = 1; // Blue
            }
        }
    }

    // Initialize Tanks
    // Assuming Tank class has methods getX(), getY()
    board[player1Tank.x * cols + player1Tank.y].character = 'T';
    board[player1Tank.x * cols + player1Tank.y].color = player1Tank.color; // Orange

    board[player2Tank.x * cols + player2Tank.y].character = 'T';
    board[player2Tank.x * cols + player2Tank.y].color = player2Tank.color; // Magenta/Purple

    // Initialize Lives (you can choose where to place them)
    // Here we assume that Tank class has a getLives() method
    // int halfScreenWidth = 150 / 2;

    // // Calculate starting positions for centered text
    // int position_for_player1 = (halfScreenWidth / 2) - (9 + player1Tank.getLives()) / 2;
    // int position_for_player2 = halfScreenWidth + (halfScreenWidth / 2) - (9 + player2Tank.getLives()) / 2;

    // // Initialize Lives
    // int text_position_for_player1 = position_for_player1 - 10; // 10 positions before the lives
    // int text_position_for_player2 = position_for_player2 - 10;

    // // Write "Player 1:" to the board
    // const std::string player1Text = "Player 1: ";
    // for (int i = 0; i < player1Text.size(); ++i)
    // {
    //     board[4 * cols + (text_position_for_player1 + i)].character = player1Text[i];
    //     board[4 * cols + (text_position_for_player1 + i)].color = 2; // Color for Player 1
    // }

    // // Write lives for Player 1
    // for (int i = 0; i < player1Tank.getLives(); ++i)
    // {
    //     board[4 * cols + (position_for_player1 + i)].character = '@';
    //     board[4 * cols + (position_for_player1 + i)].color = 4; // Red
    // }

    // // Write "Player 2:" to the board
    // const std::string player2Text = "Player 2: ";
    // for (int i = 0; i < player2Text.size(); ++i)
    // {
    //     board[4 * cols + (text_position_for_player2 + i)].character = player2Text[i];
    //     board[4 * cols + (text_position_for_player2 + i)].color = 3; // Color for Player 2
    // }

    // // Write lives for Player 2
    // for (int i = 0; i < player2Tank.getLives(); ++i)
    // {
    //     board[4 * cols + (position_for_player2 + i)].character = '@';
    //     board[4 * cols + (position_for_player2 + i)].color = 4; // Red
    // }
}
void checkGameOver(Tank &player1Tank, Tank &player2Tank, Cell *board)
{
    const int cols = 200; // Number of columns in the board

    if (player1Tank.getLives() <= 0 || player2Tank.getLives() <= 0)
    {
        // Clear the board
        for (int i = 0; i < 50 * cols; ++i)
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
        int start_position = (cols / 2) - (message.size() / 2);
        for (int i = 0; i < message.size(); ++i)
        {
            board[25 * cols + (start_position + i)].character = message[i];
            board[25 * cols + (start_position + i)].color = 1; // Any color you like for the message
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
    const int cols = 200; // Number of columns in the board

    // Clear old lives and text
    for (int i = -20; i < 20; ++i)
    { // Clear a larger area around the lives and text
        board[4 * cols + (position_for_player + i)].character = ' ';
        board[4 * cols + (position_for_player + i)].color = 0; // Default color
    }

    // Write "Player X:" to the board
    int text_position_for_player = position_for_player - playerText.size(); // Adjust based on the length of the player text

    for (int i = 0; i < playerText.size(); ++i)
    {
        board[4 * cols + (text_position_for_player + i)].character = playerText[i];
        board[4 * cols + (text_position_for_player + i)].color = playerColor;
    }

    // Draw new lives
    for (int i = 0; i < playerTank.getLives(); ++i)
    {
        board[4 * cols + (position_for_player + i)].character = '@';
        board[4 * cols + (position_for_player + i)].color = 4; // Red
    }
}

void drawBoard(Cell *board)
{
    const int rows = 50;
    const int cols = 200;

    // Iterate through the entire game board
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            // Get the Cell from the board
            Cell cell = board[i * cols + j];

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
    const int cols = 200; // Number of columns in the board

    for (size_t i = 0; i < playerTank.projectiles.size(); /* no increment */)
    {
        auto &projectile = playerTank.projectiles[i];

        // Erase old position from the board
        board[projectile->x * cols + projectile->y].character = ' ';
        board[projectile->x * cols + projectile->y].color = 0; // Default color

        // Move the projectile
        projectile->move();

        // Check for wall collision
        if (is_wall(projectile->x, projectile->y))
        {
            playerTank.removeProjectile(i);
        }
        else
        {
            // Update new position in the board
            board[projectile->x * cols + projectile->y].character = '*';
            board[projectile->x * cols + projectile->y].color = playerTank.color;

            ++i; // Only increment if the projectile wasn't removed
        }
    }
}
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

    // Set the size of the shared memory object
    const int rows = 50;
    const int cols = 200;

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
        drawPlayerLives(player1Tank, board, position_for_player1, player1Tank.color, "Player 1: ");
        drawPlayerLives(player2Tank, board, position_for_player2, player2Tank.color, "Player 2: ");

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
