#ifndef TILE_H
#define TILE_H

#include <string>

class Tile
{
public:
    Tile();
    Tile(char dchar, int moveCost, std::string name);
    ~Tile();
    char getChar();
    std::string getName();
    int getMovementCost();

private:
    char _displayChar;
    std::string _tileName;
    int _movementCost;
};

#endif
