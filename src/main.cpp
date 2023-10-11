#include <unistd.h>
#include <iostream>
#include <vector>
#include "./models/Tank.h"
#include "./models/Projectile.h"
#include <ncurses.h>

const int WALL_HEIGHT = 50;
const int WALL_WIDTH = 200;
const int INITIAL_LIVES = 3;
const int SLEEP_DURATION = 50000;

void checkCollisions(Tank &tank1, Tank &tank2)
{
    // Check if any projectile from tank1 hit tank2
    for (size_t i = 0; i < tank1.projectiles.size(); /* no increment */)
    {
        auto &projectile = tank1.projectiles[i];
        if (tank2.isHit(projectile->x, projectile->y))
        {
            tank2.decreaseLife();      // Decrease the life of the tank that got hit
            tank1.removeProjectile(i); // Remove the projectile that caused the hit
        }
        else
        {
            ++i;
        }
    }

    // Check if any projectile from tank2 hit tank1
    for (size_t i = 0; i < tank2.projectiles.size(); /* no increment */)
    {
        auto &projectile = tank2.projectiles[i];
        if (tank1.isHit(projectile->x, projectile->y))
        {
            tank1.decreaseLife();      // Decrease the life of the tank that got hit
            tank2.removeProjectile(i); // Remove the projectile that caused the hit
        }
        else
        {
            ++i;
        }
    }
}
void displayVictoryMessage(int winnerPlayerColor, const std::string &winnerPlayer)
{
    erase(); // Clear the screen

    attron(COLOR_PAIR(winnerPlayerColor));
    mvprintw(25, 75, (winnerPlayer + " won the game!").c_str()); // Display the message at approximately the center of the screen
    attroff(COLOR_PAIR(winnerPlayerColor));
    refresh();

    // Option 1: Delay Method
    usleep(5000000); // Sleep for 5 seconds

    // Option 2: Key Press Method
    mvprintw(26, 75, "Press any key to exit...");

    refresh();
    getch(); // Wait for a key press
}

void drawLayout(int player1Lives, int player2Lives)
{
    // Draw the walls
    attron(COLOR_PAIR(1));
    for (int i = 10; i < 50; ++i)
    {
        for (int j = 0; j < 200; ++j)
        {
            if (is_wall(i, j))
            {
                mvprintw(i, j, "#");
            }
        }
    }
    attroff(COLOR_PAIR(1));

    // Centering adjustments
    int halfScreenWidth = 150 / 2;

    // Calculate starting positions for centered text
    int player1Start = (halfScreenWidth / 2) - (9 + player1Lives) / 2; // 9 for "Player 1: "
    int player2Start = halfScreenWidth + (halfScreenWidth / 2) - (9 + player2Lives) / 2;

    // Draw Player 1's lives
    attron(COLOR_PAIR(2));                   // Color for player 1
    mvprintw(4, player1Start, "Player 1: "); // Moved to row 5 (4 in 0-based)
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(4)); // Color for lives
    for (int i = 0; i < player1Lives; ++i)
    {
        mvprintw(4, player1Start + 9 + i, "@"); // Moved to row 5 (4 in 0-based)
    }
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(3)); // Color for player 2
    mvprintw(4, player2Start, "Player 2: ");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4)); // Color for lives
    for (int i = 0; i < player2Lives; ++i)
    {
        mvprintw(4, player2Start + 9 + i, "@");
    }
    attroff(COLOR_PAIR(4));
}
void handleProjectiles(Tank &playerTank)
{
    for (size_t i = 0; i < playerTank.projectiles.size(); /* no increment */)
    {
        auto &projectile = playerTank.projectiles[i];

        // Erase old position
        mvprintw(projectile->x, projectile->y, " ");

        // Move
        projectile->move();

        // Check for wall collision
        if (is_wall(projectile->x, projectile->y))
        {
            playerTank.removeProjectile(i);
        }
        else
        {
            // Draw new position
            attron(COLOR_PAIR(playerTank.color));
            mvprintw(projectile->x, projectile->y, "*");
            attroff(COLOR_PAIR(playerTank.color));
            ++i;
        }
    }
}
int main()
{
    // Initialize ncurses
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Makes getch non-blocking
    start_color();

    ////////////////////////////////////
    init_pair(1, COLOR_BLUE, COLOR_BLACK);    // Wall color
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // First tank and its projectiles
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // Projectile color
    init_pair(4, COLOR_RED, COLOR_BLACK);
    ////////////////////////////////////
    noecho();
    cbreak();
    curs_set(0);

    // Debugging: Validate ncurses initialization
    if (has_colors() == FALSE)
    {
        endwin();
        std::cerr << "Your terminal does not support color\\n";
        return 1;
    }

    // Initialize tank
    Tank player1Tank(1, 25, 50, 2, 3); // ID 1, Green
    Tank player2Tank(2, 25, 150, 3, 3);

    // Vector to hold multiple projectiles
    std::vector<Projectile *> projectiles;

    while (true)
    {
        if (player1Tank.lives <= 0)
        {
            displayVictoryMessage(player2Tank.color, "Player 2");
            break;
        }
        else if (player2Tank.lives <= 0)
        {
            displayVictoryMessage(player1Tank.color, "Player 1");
            break;
        }
        // Clear screen
        erase();

        drawLayout(player1Tank.getLives(), player2Tank.getLives());
        // Draw walls
        attron(COLOR_PAIR(1));

        attroff(COLOR_PAIR(1));

        // Debugging: Check the position of the tank before drawing
        if (player1Tank.x <= 0 || player1Tank.y <= 0 || player2Tank.x <= 0 || player2Tank.y <= 0)
        {
            endwin();
            std::cerr << "Tank is out of bounds\\n";
            return 1;
        }

        // Draw tank
        attron(COLOR_PAIR(player1Tank.color));
        mvprintw(player1Tank.x, player1Tank.y, "T");
        attroff(COLOR_PAIR(player1Tank.color));

        attron(COLOR_PAIR(player2Tank.color));
        mvprintw(player2Tank.x, player2Tank.y, "T");
        attroff(COLOR_PAIR(player2Tank.color));

        // Handle shooting and moving
        int ch = getch();
        flushinp();
        if (ch == 'q')
        {
            break;
        }
        else if (ch == 10)
        {
            player2Tank.shoot();
        }
        else if (ch == 32)
        { // Space key to shoot for player2
            player1Tank.shoot();
        }
        else if (ch != ERR)
        {
            player1Tank.move(ch);
            player2Tank.move(ch);
        }

        // Move and draw projectiles
        handleProjectiles(player1Tank);
        handleProjectiles(player2Tank);
        checkCollisions(player1Tank, player2Tank);
        // Refresh screen
        refresh();

        // Sleep for 100ms to slow down the loop
        usleep(50000);
    }

    // Clean up
    // for (auto projectile : projectiles)
    // {
    //     delete projectile;
    // }
    endwin();
    return 0;
}