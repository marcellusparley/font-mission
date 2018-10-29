#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <utility>
#include "Map.h"

// Constructor. Right now just setting default tiles and unit for testing
// need to implement creation of map based on a file of some kind.
Map::Map()
{
    _filename = "none";
    _height = 100;
    _width = 100;
    
    Tile* t = new Tile();
    for (int i=0; i < (_width*_height); i++)
        _tilemap.push_back(*t);

    for (int i=0; i < (_width*_height); i++)
        _unitmap.push_back(nullptr);

    for (int i=0; i < (_width*_height); i++)
        _highlights.push_back(1);
    
    _unitmap[50] = new Unit();
    _unitmap[50+_width] = new Unit();
    
    delete t;
}

// Map constructor that parses a map file. Expand on this. Currently only
// does simple placing of default tile and units.
Map::Map(std::string filename)
{
    _filename = filename;
    std::ifstream in(filename);
    std::string line;
    std::vector<Tile*> tempTiles;
    std::vector<std::string> tempUnits;

    in >> _width >> _height;
    in.get();
    in.get();

    std::getline(in, line);
    std::cout << "Got line: " << line << "\n";
    if (line == "TILES")
    {
        char display;
        int mcost;
        std::string tname;

        std::getline(in,line);
        std::cout << "Got line: " << line << "\n";
        while (!line.empty())
        {
            std::istringstream iss(line);
            iss >> display >> mcost >> tname;
            tempTiles.push_back(new Tile(display, mcost, tname));
            std::getline(in, line);
            std::cout << "Got line: " << line << "\n";
        }

    }

    std::getline(in, line);
    std::cout << "Got line: " << line << "\n";
    if (line == "UNITS")
    {

        std::getline(in, line);
        std::cout << "Got line: " << line << "\n";
        while (!line.empty())
        {
            tempUnits.push_back(line);
            std::getline(in,line);
            std::cout << "Got line: " << line << "\n";
        }

        for (auto s : tempUnits)
            std::cout << s << "\n";
        for (int i=0; i < tempTiles.size()-1; i++)
            std::cout << tempTiles[i]->getChar() << "\n";
    }

    std::getline(in, line);
    std::cout << "Got line: " << line << "\n";
    if (line == "DATA")
    {
        std::getline(in, line);
        std::cout << "Got line: " << line << "\n";
        for (char ch : line)
        {
            if (ch >= '0' && ch <= '9')
            {
                int ich = ch - '0';
                _tilemap.push_back(*tempTiles[ich]);
            }
        }

        
        std::getline(in, line);
        std::cout << "Got line: " << line << "\n";
        for (char ch : line)
        {
            int fact, mv, maxMv, hlth, att;
            char display;
            std::string uname;
            if (ch == '0')
                _unitmap.push_back(nullptr);
            else if (ch > '0' && ch <= '9')
            {
                int ich = ch - '0';
                ich--;
                
                std::istringstream iss(tempUnits[ich]);
                iss >> fact >> display >> uname >> mv >> maxMv >> hlth >> att;

                _unitmap.push_back(new Unit(fact, display, uname, mv, maxMv,
                            hlth, att));
            }
        }
    }

    std::cout << "pushing highlights\n";
    for (int i=0; i < (_width*_height); i++)
        _highlights.push_back(1);
    std::cout << "highlights pushed\ncleaning tiles\n";

    for (int i=0; i < tempTiles.size(); i++)
        delete tempTiles[i];

    tempTiles.clear();

    std::cout << "tiles cleaned\ncleaning units\n";

    /*
    for (int i=0; i < tempUnits.size()-1; i++)
        delete tempUnits[i];
    */

   // tempUnits.clear();

    std::cout << "units cleaned\n";
    
}

Map::~Map() 
{
    for (int i=0; i < (_width*_height); i++)
    {
        delete _unitmap[i];
    }
    _unitmap.clear();
}

int Map::getWidth() { return _width; }
int Map::getHeight() { return _height; }

// Getting index for location above and below and converting 2d coords to index
int Map::_iAbove(int index) { return index - _width; }
int Map::_iBelow(int index) { return index + _width; }
int Map::_toIndex(int x, int y) { return x + (y*_width); }

// Getting 2d values for an index location
int Map::_getX(int index)
{
    if (index == 0) return 0;
    int x = index % _width;
    return x;
}

int Map::_getY(int index)
{
    return index / _width;
}

// Returns a pointer to unit at a 2d location on map
Unit* Map::getUnit(int x, int y) 
{ 
    return _unitmap[x + (y*_width)]; 
}

// Get the highlight value for the map location. These values coorespond to the
// ncurses color pairs.
int Map::getHighlight(int x, int y)
{
    return _highlights[x + (y*_width)];
}

// Highlight spaces available to move when a unit is selected. Argument is
// location of the unit
void Map::_moveHighlight(int index)
{
    // If a unit isn't there don't do anything
    if (_unitmap[index] == nullptr)
        return;

    // The following is a basic BFS on the map using the unit's location
    // and move points left
    int moves = _unitmap[index]->getMovePoints();
    std::queue<std::pair<int,int>> tileQueue;
    tileQueue.push(std::make_pair(index, moves));

    // While there are spaces to be evaluated you need to pop off a location
    // and the cooresponding moves left from the queue and highlight, decrement
    // the moves, and push the next locations
    while (!tileQueue.empty())
    {
        // Gets the front of queue's values and pops it off
        int currIndex = tileQueue.front().first;
        int currMoves = tileQueue.front().second;
        tileQueue.pop();

        // If out of moves don't highlight it; just continue with the loop
        if (currMoves < 0) continue;
            
        // Highlight the space, add it to the visited dictionary, and decrement
        // the current moves so you can add the next batch of tiles to the 
        // queue
        _highlights[currIndex] = 2;
        _visited[currIndex] = currMoves;
        currMoves -= _tilemap[currIndex].getMovementCost();

        // Adding the neighbors making sure they are actually inside the map
        if ( (_getX(currIndex) > 0) )
            if (_highlights[currIndex-1] != 2)
                tileQueue.push(std::make_pair(currIndex-1, currMoves));
        if ( (_getX(currIndex) < _width-1) )
            if (_highlights[currIndex+1] != 2)
                tileQueue.push(std::make_pair(currIndex+1, currMoves));
        if ( (_getY(currIndex) > 0) )
            if ( _highlights[currIndex-_width] != 2 )
                tileQueue.push(std::make_pair(currIndex-_width, currMoves));
        if ( (_getY(currIndex) < _height-1) )
            if ( _highlights[currIndex+_width] != 2 )
                tileQueue.push(std::make_pair(currIndex+_width, currMoves));
    }
}
    
// Get the tile at a given 2d location
Tile Map::getTile(int x, int y)
{
    int index = (y * _width) + x;
    return _tilemap[index];
}

// Clears all the current highlights the value 1 cooresponds to the default
// ncurses colorpair
void Map::_resetHighlights()
{
    _visited.clear();
    for (int i=0; i < _width*_height; i++)
        _highlights[i] = 1;
}

// This is for when the user selects a unit, it just makes sure the unit is at
// that location and calls the highlighting function
bool Map::select(int x, int y)
{
    if (_unitmap[x + (y * _width)] == nullptr)
    {
        _resetHighlights();
        return false;
    }

    // _highlights[(x+1) + ((y+1) * _width)] = 2;
    _moveHighlight(x + (y * _width));
    return true;
}

// This actually moves the unit in the unitmap given a source 2d location and
// a target 2d location
bool Map::moveUnit(int srcX, int srcY, int destX, int destY)
{
    // This checks that the _unitmap location isn't null (that there is a unit
    // at that location), if the destination is available to move (by checking
    // the current highlight), and that the source and destinations aren't the
    // same. If any of these are true it resets the highlights and returns.
    if (_unitmap[srcX + (srcY * _width)] == nullptr ||
            _highlights[destX + (destY*_width)] != 2 ||
            (srcX == destX && srcY == destY))
    {
        _resetHighlights();
        return false;
    }

    // This decreases the move points of that unit based on the cost 
    // determined during the BFS for that dest tile, swaps the pointers
    // between the locations, and clears the highlights.
    _unitmap[srcX+(srcY*_width)]->setMove(_visited[destX+(destY*_width)]);
    _unitmap[destX + (destY*_width)] = _unitmap[srcX+(srcY*_width)];
    _unitmap[srcX+(srcY*_width)] = nullptr;
    _resetHighlights();
    return true;
}

// This is a debug function for reseting unit movecounts simulating a new turn
void Map::resetMoves()
{
    for (auto p : _unitmap)
    {
        if (p != nullptr)
            p->resetMovePoints();
    }
}

// This is a method that will be called when a unit attacks another. It is
// given the location of the sorce and target units. If either is not a 
// unit it will exit without attacking.
bool Map::attackUnit(int srcX, int srcY, int targetX, int targetY)
{
    // This makes sure both units exist and that they aren't the same
    // Will add in a attack distance check later
    if (_unitmap[srcX + (srcY * _width)] == nullptr ||
            _unitmap[targetX + (targetY * _width)] == nullptr || 
            (srcX == targetX && srcY == targetY))
    {
        _resetHighlights();
        return false;
    }

    // Get source unit's attack and damage target
    // Will need to be refactored later to implement desired calculations
    int attack = _unitmap[srcX+(srcY*_width)]->getAttack();
    _unitmap[targetX+(targetY*_width)]->damageHealth(attack);
    _resetHighlights();
    return true;
}
