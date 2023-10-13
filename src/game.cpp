#include "Game.h"
#include <ncurses.h>
#include <iostream>
#include <unistd.h>

// Constructor
Game::Game() : player1Tank(1, 25, 50, 2, INITIAL_LIVES), player2Tank(2, 25, 150, 3, INITIAL_LIVES)
{
}

// Destructor
Game::~Game()
{
    endwin(); // End ncurses mode
}

// Initialize the game
void Game::initialize()
{
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Makes getch non-blocking
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);    // Wall color
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // First tank and its projectiles
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // Second tank and its projectiles
    init_pair(4, COLOR_RED, COLOR_BLACK);     // Lives
    noecho();
    cbreak();
    curs_set(0);

    // Validate ncurses initialization
    if (has_colors() == FALSE)
    {
        endwin();
        std::cerr << "Your terminal does not support color\\n";
        exit(1);
    }
}
void Game::checkCollisions()
{
    // Check if any projectile from player1Tank hit player2Tank
    for (size_t i = 0; i < player1Tank.projectiles.size(); /* no increment */)
    {
        auto &projectile = player1Tank.projectiles[i];
        if (player2Tank.isHit(projectile->x, projectile->y))
        {
            player2Tank.decreaseLife();      // Decrease the life of the tank that got hit
            player1Tank.removeProjectile(i); // Remove the projectile that caused the hit
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
            player1Tank.decreaseLife();      // Decrease the life of the tank that got hit
            player2Tank.removeProjectile(i); // Remove the projectile that caused the hit
        }
        else
        {
            ++i;
        }
    }
}
void Game::run()
{
    // Initialize the game
    initialize();

    // Main game loop
    while (true)
    {
        // Game logic
        update();

        // Check for collisions
        checkCollisions();

        if (player1Tank.getLives() <= 0)
        {
            displayVictoryMessage(player2Tank.color, "Player 2");
            break; // Exit the game loop
        }
        else if (player2Tank.getLives() <= 0)
        {
            displayVictoryMessage(player1Tank.color, "Player 1");
            break; // Exit the game loop
        }
        else
            // Render game state
            draw();
    }
}

void Game::update()
{
    // Handle user input for player 1 and player 2
    int ch = getch();
    flushinp();
    // if (ch == 'q')
    // {
    //     break;
    // }
    // else
    if (ch == 10)
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

    // Update the positions of projectiles
    handleProjectiles(player1Tank);
    handleProjectiles(player2Tank);
    checkCollisions();
}

void Game::draw()
{
    // Clear the screen
    erase();

    // Draw game layout (walls, scoreboards, etc.)
    drawLayout();

    // Draw tanks
    player1Tank.draw();
    player2Tank.draw();

    // Draw projectiles for both tanks
    for (const auto &projectile : player1Tank.projectiles)
    {
        projectile->draw();
    }
    for (const auto &projectile : player2Tank.projectiles)
    {
        projectile->draw();
    }

    // Update the screen
    refresh();
    usleep(SLEEP_DURATION);
}
void Game::handleProjectiles(Tank &playerTank)
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
void Game::displayVictoryMessage(int winnerPlayerColor, const std::string &winnerPlayer)
{
    clear();
    attron(COLOR_PAIR(winnerPlayerColor));
    mvprintw(WALL_HEIGHT / 2, WALL_WIDTH / 2, "Player %s Wins!", winnerPlayer.c_str());
    attroff(COLOR_PAIR(winnerPlayerColor));
    refresh();
    refresh();
    refresh();
    usleep(3000000);
    getch();
}

void Game::drawLayout()
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
    int player1Start = (halfScreenWidth / 2) - (9 + player1Tank.getLives()) / 2; // 9 for "Player 1: "
    int player2Start = halfScreenWidth + (halfScreenWidth / 2) - (9 + player2Tank.getLives()) / 2;

    // Draw Player 1's lives
    attron(COLOR_PAIR(2));                   // Color for player 1
    mvprintw(4, player1Start, "Player 1: "); // Moved to row 5 (4 in 0-based)
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(4)); // Color for lives
    for (int i = 0; i < player1Tank.getLives(); ++i)
    {
        mvprintw(4, player1Start + 9 + i, "@"); // Moved to row 5 (4 in 0-based)
    }
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(3)); // Color for player 2
    mvprintw(4, player2Start, "Player 2: ");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4)); // Color for lives
    for (int i = 0; i < player2Tank.getLives(); ++i)
    {
        mvprintw(4, player2Start + 9 + i, "@");
    }
    attroff(COLOR_PAIR(4));
}
