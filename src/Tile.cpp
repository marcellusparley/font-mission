#include "Tile.h"

// Constructor this will be refactored later to be more generic and allow the
// Map object which will be handling it's creation to set all these values as
// arugment
Tile::Tile()
{
    _displayChar = '.';
    _movementCost = 1;
    _tileName = "test";
}

Tile::Tile(char dchar, int moveCost, std::string name)
{
    _displayChar = dchar;
    _movementCost = moveCost;
    _tileName = name;
}

Tile::~Tile()
{}

// Gets the display character
char Tile::getChar()
{
    return _displayChar;
}

// I'm thinking each tile will have a name that can be displayed if there is 
// some kind of tile info display feature later
std::string Tile::getName()
{
    return _tileName;
}

// This is the cost to move on this tile
int Tile::getMovementCost()
{
    return _movementCost;
}
