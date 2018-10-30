#include "Game.h"
#include <ncurses.h>
#include <string>

Game::Game()
{
    // _map = new Map();
    _map = new Map("maps/map0.txt");
    QUIT = 0;
    _cursor.x = 0;
    _cursor.y = 1;
    _off.x = 0;
    _off.y = -1;
    _frame.w = 10;
    _frame.h = 10;
    _frame.x = 0;
    _frame.y = 1;
    _ren = new Renderer(_map, _cursor, _frame, _off);
    _selected.x = -1;
    _selected.y = -1;
}

Game::~Game()
{
    delete _map;
    delete _ren;
}

void Game::cleanup()
{
    _ren->cleanup();
}

// Get the real values for the cursor postions
int Game::_realX() { return _cursor.x + _off.x; }
int Game::_realY() { return _cursor.y + _off.y; }

// Simple gameloop
void Game::gameLoop()
{
    clear();
    _ren->render();

    while (!QUIT)
    {
        move(_cursor.y, _cursor.x);
        handleInput();
    }
}

// Input handling, big mess rn probably should divvie it out to separate
// objects or maybe states if I want to
void Game::handleInput()
{
    char ch = getch();

    switch (ch)
    {
    case 'h':
    case KEY_LEFT:
        if (_realX() > 0)
        {
            // If it hits the scroll map edge then move the offset/scroll
            // the map, else just move the cursor on the screen
            if (_realX() == _off.x + 2)
            {
                _off.x -= 1;
                _ren->render();
            }
            else
            {
                _cursor.x -= 1;
                move(_cursor.y, _cursor.x);
            }
            refresh();
        }
        break;
    case 'l':
    case KEY_RIGHT:
        if (_realX() < _map->getWidth()-1)
        {
            // Same as above, handling the scrol edge and moving
            if (_realX() == _off.x + _frame.w - 3)
            {
                _off.x += 1;
                _ren->render();
            }
            else
            {
                _cursor.x += 1;
                move(_cursor.y, _cursor.x);
            }
            refresh();
        }
        break;
    case 'k':
    case KEY_UP:
        if (_realY() > 0)
        {
            if (_realY() == _off.y + 2)
            {
                _off.y -= 1;
                _ren->render();
            }
            else
            {
                _cursor.y -= 1;
                move(_cursor.y, _cursor.x);
            }
            refresh();
        }
        break;
    case 'j':
    case KEY_DOWN:
        if (_realY() < _map->getHeight()-1)
        {
            if (_realY() == _off.y + _frame.h - 3)
            {
                _off.y += 1;
                _ren->render();
            }
            else
            {
                _cursor.y += 1;
                move(_cursor.y, _cursor.x);
            } refresh(); }
        break;
    // Quiting the game
    case 'q':
    case 'Q':
        QUIT = true;
        break;

    // Rightnow this is a big case for selecting a unit for movement, 
    // displaying stats, and moving the unit. Need to refactor this shit
    // for sure
    case 'm':

        // When nothing is currently selected and if there is a unit on the
        // current location, get that unit and display the stats
        if (_selected.x == -1)
        {
            if (_map->select(_realX(), _realY()))
            {
                _selected.x = _realX();
                _selected.y = _realY();
                //clear();
                std::string stat = _map->getUnit(_realX(), _realY())->getStatus();
                mvaddstr(0, COLS/2, stat.c_str());
                move(_cursor.y, _cursor.x);
                _ren->render();
            }
        }

        // Else if something is already selected try moving. Unit will move
        // if the position is open and not the selected postion but that is
        // checked in the move code itself. Regardless it toggles off the 
        // stats display
        else
        {
            bool t=_map->moveUnit(_selected.x, _selected.y, _realX(), _realY());
            _selected.x = -1;
            _selected.y = -1;
            mvaddstr(0, COLS/2, "                      ");
            move(_cursor.y, _cursor.x);
            //clear();
            _ren->render();
        }
        break;

    // This case is a super simple and basic attack functionality
    case 'a':
        if (_selected.x != -1)
        {
            bool test = _map->attackUnit(_selected.x, _selected.y, _realX(), _realY());
            _selected.x = -1;
            _selected.y = -1;
            mvaddstr(0, COLS/2, "                      ");
            move(_cursor.y, _cursor.x);
            _ren->render();
        }

        break;

    // Debug key to reset the moves of all the units on the map, take out
    // when you implement turns and shit.
    case 't':
        _map->resetMoves();
        break;
    case 'r':
        clear();
        _ren->render();
        refresh();
        break;
    default:
        break;
    }
}

void Game::init()
{
    _ren->init();
}
