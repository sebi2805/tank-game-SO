#include <unistd.h>
#include <iostream>
#include <vector>
#include "./models/Tank.h"
#include "./models/Projectile.h"
#include <ncurses.h>

void drawWall()
{
    attron(COLOR_PAIR(1));
    for (int i = 0; i < 50; ++i)
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
    Tank player1Tank(1, 25, 50, 2); // ID 1, Green
    Tank player2Tank(2, 25, 150, 3);

    // Vector to hold multiple projectiles
    std::vector<Projectile *> projectiles;

    while (true)
    {
        // Clear screen
        erase();

        drawWall();
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
        // Refresh screen
        refresh();

        // Sleep for 100ms to slow down the loop
        usleep(100000);
    }

    // Clean up
    // for (auto projectile : projectiles)
    // {
    //     delete projectile;
    // }
    endwin();
    return 0;
}