#ifndef MAP_H
#define MAP_H

#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Unit.h"
#include "Tile.h""

enum HL_COLORS {Normal=1, Movement};

class Map
{
public:
    Map();
    Map(std::string filename);
    ~Map();
    int getWidth();
    int getHeight();
    Tile getTile(int x, int y);
    Unit* getUnit(int x, int y);
    int getHighlight(int x, int y);
    bool select(int x, int y);
    bool moveUnit(int srcX, int srcY, int destX, int destY);
    bool attackUnit(int srcX, int srcY, int targetX, int targetY);
    void resetMoves();

private:
    std::string _filename;
    // Vectors hold the different maps in 1D structure making conversion
    // between the 2d representation and 1d implementation probably need to 
    // fiddle with the implementation of units
    std::vector<Tile> _tilemap;
    std::vector<Unit*> _unitmap;
    std::vector<int> _highlights;
    std::unordered_map<int, int> _visited;

    // Handleing the highlighting of tiles when a unit is selected
    void _resetHighlights();
    void _moveHighlight(int index);

    // Converting 2d to 1d getting the index from a pair of coords and getting
    // the index above and below current index
    int _toIndex(int x, int y);
    int _iAbove(int index);
    int _iBelow(int index);
    int _normX(int index);
    int _getX(int index);
    int _getY(int index);

    // Dimensions of map
    int _height;
    int _width;
};


#endif
