#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <utility>
#include <string>
#include "Map.h"

class Game
{
public:
    Game();
    ~Game();
    void init();
    void cleanup();
    void gameLoop();
    void render();
    void handleInput();

private:
    Map* _map;

    // This is to hold location selected when user selects a unit for
    // movement or attack
    std::pair<int, int> _selected;

    // Bool for the gameloop
    bool QUIT;

    // Cursor location on screen
    int _cX;
    int _cY;

    // Frame dimensions of map display
    int _fW;
    int _fH;
    
    // Origin of map display frame
    int _winX;
    int _winY;

    // Offset of map
    int _offX;
    int _offY;

    // Get the actual map values of the cursor values
    // Probably should make it take no arguments 
    int _realX(int cx);
    int _realY(int cy);
};


#endif
