#include "Renderer.h"
#include <ncurses.h>

Renderer::Renderer(Map* map, Loc& curs, Rect& frame, Loc& off)
{
    _map = map;
    _curs = &curs;
    _off = &off;
    _frame = &frame;
}

Renderer::~Renderer() {}

void  Renderer::init()
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
    _frame->w=COLS;
    _frame->h=LINES-1;
}

void Renderer::cleanup()
{
    endwin();
}

void Renderer::render()
{
    // Moves the cursor to the top left of the terminal and writes out simple
    // debug info (currently just the offset values for the map)
    move(0,0);
    std::string locline = "offX: " + std::to_string(_off->x) +
        " offY: " + std::to_string(_off->y) + "  ";
    addstr(locline.c_str());

    // Loop that handles printing everything on the map
    for (int y = _frame->y; y < _frame->h; y++)
        for (int x=_frame->x; x < _frame->w; x++)
        {
            if (x+_off->x >= _map->getWidth() || y+_off->y >= _map->getHeight() ||
                    x+_off->x < 0 || y+_off->y < 0)
            {
                attron(COLOR_PAIR(1));
                mvaddch(y, x, '#');
                attron(COLOR_PAIR(1));
            }
            else if (_map->getUnit(x+_off->x, y+_off->y) != nullptr)
            {
                attron(COLOR_PAIR(_map->getHighlight(x+_off->x, y+_off->y)));
                mvaddch(y, x, _map->getUnit(x+_off->x, y+_off->y)->getChar());
                attron(COLOR_PAIR(_map->getHighlight(x+_off->x, y+_off->y)));
            }
            else
            {
                attron(COLOR_PAIR(_map->getHighlight(x+_off->x, y+_off->y)));
                mvaddch(y, x, _map->getTile(x+_off->x, y+_off->y).getChar());
                attron(COLOR_PAIR(_map->getHighlight(x+_off->x, y+_off->y)));
            }
        }
}
