#include "Game.h"

Game::Game()
{
    // _map = new Map();
    _map = new Map("maps/map0.txt");
    QUIT = 0;
    _cX = 0;
    _cY = 1;
    _fW = 10;
    _fH = 10;
    _offX = 0;
    _offY = -1;
    _winX = 0;
    _winY = 1;
    _selected = std::make_pair(-1, -1);
}

Game::~Game()
{
    delete _map;
    cleanup();
}

void Game::render()
{
    // Moves the cursor to the top left of the terminal and writes out simple
    // debug info (currently just the offset values for the map)
    move(0,0);
    std::string locline = "offX: " + std::to_string(_offX) +
        " offY: " + std::to_string(_offY) + "  ";
    addstr(locline.c_str());

    // Loop that handles printing everything on the map
    for (int y = _winY; y < _fH; y++)
        for (int x=_winX; x < _fW; x++)
        {
            if (x+_offX >= _map->getWidth() || y+_offY >= _map->getHeight() ||
                    x+_offX < 0 || y+_offY < 0)
            {
                attron(COLOR_PAIR(1));
                mvaddch(y, x, '#');
                attron(COLOR_PAIR(1));
            }
            else if (_map->getUnit(x+_offX, y+_offY) != nullptr)
            {
                attron(COLOR_PAIR(_map->getHighlight(x+_offX, y+_offY)));
                mvaddch(y, x, _map->getUnit(x+_offX, y+_offY)->getChar());
                attron(COLOR_PAIR(_map->getHighlight(x+_offX, y+_offY)));
            }
            else
            {
                attron(COLOR_PAIR(_map->getHighlight(x+_offX, y+_offY)));
                mvaddch(y, x, _map->getTile(x+_offX, y+_offY).getChar());
                attron(COLOR_PAIR(_map->getHighlight(x+_offX, y+_offY)));
            }
        }
}

// Get the real values for the cursor postions
int Game::_realX(int cx) { return cx + _offX; }
int Game::_realY(int cy) { return cy + _offY; }

// Initialize everything
void Game::init()
{    
    // initialize ncurses
	initscr();
    start_color();
	clear();
	noecho();
	// cbreak();
	keypad(stdscr, TRUE);

    // Color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_RED);

    // ncurse GLOBALS for the terminal dimesnisions
    _fW=COLS;
    _fH=LINES-1;
}

// Simple gameloop
void Game::gameLoop()
{
    while (!QUIT)
    {
        move(_cY, _cX);
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
        if (_realX(_cX) > 0)
        {
            // If it hits the scroll map edge then move the offset/scroll
            // the map, else just move the cursor on the screen
            if (_realX(_cX) == _offX + 2)
            {
                _offX -= 1;
                render();
            }
            else
            {
                _cX -= 1;
                move(_cY, _cX);
            }
            refresh();
        }
        break;
    case 'l':
    case KEY_RIGHT:
        if (_realX(_cX) < _map->getWidth()-1)
        {
            // Same as above, handling the scrol edge and moving
            if (_realX(_cX) == _offX + _fW - 3)
            {
                _offX += 1;
                render();
            }
            else
            {
                _cX += 1;
                move(_cY, _cX);
            }
            refresh();
        }
        break;
    case 'k':
    case KEY_UP:
        if (_realY(_cY) > 0)
        {
            if (_realY(_cY) == _offY + 2)
            {
                _offY -= 1;
                render();
            }
            else
            {
                _cY -= 1;
                move(_cY, _cX);
            }
            refresh();
        }
        break;
    case 'j':
    case KEY_DOWN:
        if (_realY(_cY) < _map->getHeight()-1)
        {
            if (_realY(_cY) == _offY + _fH - 3)
            {
                _offY += 1;
                render();
            }
            else
            {
                _cY += 1;
                move(_cY, _cX);
            }
            refresh();
        }
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
        if (_selected.first == -1)
        {
            if (_map->select(_realX(_cX), _realY(_cY)))
            {
                _selected.first = _realX(_cX);
                _selected.second = _realY(_cY);
                //clear();
                std::string stat = _map->getUnit(_realX(_cX), _realY(_cY))->getStatus();
                mvaddstr(0, COLS/2, stat.c_str());
                move(_cY, _cX);
                render();
            }
        }

        // Else if something is already selected try moving. Unit will move
        // if the position is open and not the selected postion but that is
        // checked in the move code itself. Regardless it toggles off the 
        // stats display
        else
        {
            bool test = _map->moveUnit(_selected.first, _selected.second, _realX(_cX), _realY(_cY));
            _selected.first = -1;
            _selected.second = -1;
            mvaddstr(0, COLS/2, "                      ");
            move(_cY, _cX);
            //clear();
            render();
        }
        break;

    // This case is a super simple and basic attack functionality
    case 'a':
        if (_selected.first != -1)
        {
            bool test = _map->attackUnit(_selected.first, _selected.second, _realX(_cX), _realY(_cY));
            _selected.first = -1;
            _selected.second = -1;
            mvaddstr(0, COLS/2, "                      ");
            move(_cY, _cX);
            render();
        }

        break;

    // Debug key to reset the moves of all the units on the map, take out
    // when you implement turns and shit.
    case 't':
        _map->resetMoves();
        break;
    case 'r':
        clear();
        render();
        refresh();
        break;
    default:
        break;
    }
}

void Game::cleanup()
{
    endwin();
}
