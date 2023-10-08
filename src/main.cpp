#include <ncurses.h>
#include <vector>
#include "./models/Tank.h"
#include "./models/Projectile.h"
#include <stdexcept>
#include <ostream>
#include <iostream>

int main()
{
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    timeout(100); // Non-blocking input
    start_color();

    // Initialize color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Tank 1 color
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Tank 2 color
    init_pair(3, COLOR_BLUE, COLOR_BLACK);   // Wall color

    // Initialize tanks
    std::vector<Tank> tanks;
    try
    {
        tanks.push_back(Tank(1, 10, 10, 1)); // Green tank
        tanks.push_back(Tank(2, 20, 20, 2)); // Yellow tank
    }
    catch (const std::runtime_error &e)
    {
        endwin();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Main game loop
    while (true)
    {
        // Clear the screen
        clear();

        // Handle user input and tank actions
        int ch = getch();
        if (ch == 'q')
        {
            break; // Quit the game
        }

        for (auto &tank : tanks)
        {
            tank.move(ch);

            // if (tank.shoot(ch))
            // {
            tank.shoot();
            // }

            // Move and draw each tank's projectiles
            for (int i = 0; i < tank.projectiles.size(); ++i)
            {
                Projectile *projectile = tank.projectiles[i];

                // Erase the old position of the projectile
                attron(COLOR_PAIR(projectile->color));
                mvprintw(projectile->x, projectile->y, " ");
                attroff(COLOR_PAIR(projectile->color));

                // Move the projectile
                projectile->move();

                // Check for collisions with walls
                if (is_wall(projectile->x, projectile->y))
                {
                    tank.removeProjectile(i);
                    --i; // Adjust index due to removal
                    continue;
                }

                // Draw the new position of the projectile
                attron(COLOR_PAIR(projectile->color));
                mvprintw(projectile->x, projectile->y, "*");
                attroff(COLOR_PAIR(projectile->color));

                // ...additional logic to check for collisions with tanks
            }
            // Draw tank
            attron(COLOR_PAIR(tank.color));
            mvprintw(tank.x, tank.y, "T");
            attroff(COLOR_PAIR(tank.color));
        }

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

        refresh(); // Update the screen
    }

    // Cleanup
    for (auto &tank : tanks)
    {
        for (auto *projectile : tank.projectiles)
        {
            delete projectile;
        }
    }

    endwin();
    return 0;
}
