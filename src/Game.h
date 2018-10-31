#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Tools.h"
#include "Renderer.h"

class Game
{
public:
    Game();
    ~Game();
    void gameLoop();
    void handleInput();
    void init();
    void cleanup();

private:
    Map* _map;
    Renderer* _ren;

    // This is to hold location selected when user selects a unit for
    // movement or attack
    Loc _selected;

    // Bool for the gameloop
    bool QUIT;

    // Cursor location on screen
    Loc _cursor;

    // Frame dimensions of map display
    Rect _frame;

    // Offset of map
    Loc _off;

    // Get the actual map values of the cursor values
    // Probably should make it take no arguments 
    int _realX();
    int _realY();
};


#endif
