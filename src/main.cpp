#include <ncurses.h>
#include <unistd.h>
#include "Game.h"

void erase(int y, int x);

int main(int argc, char *argv[])
{
    // Create and initialize game object
    Game* g = new Game();
    g->init();

	// Printing a welcom message on screen
	printw("h,j,k,l or arrow keys to move,\nm to select a unit for movement, \nt to reset the turn/reset movepoints, \nand q to quit\n\npress any key to start");

	// Wait until the user presses a key
	int ch = getch();

	// Clear the screen and do an initial render
	clear();
    g->render();

    // Starts the game loop
    g->gameLoop();

    // Cleans up game object and exits program
    g->cleanup();
    delete g;
	return 0;
}

