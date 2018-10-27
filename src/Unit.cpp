#include "Unit.h"

// Constructor currently just a stand in to have something usable right now
// will be refactored later so that the Map file parser or Map object or
// whover will handle creation and will be able to pass these values in
// as arguments
Unit::Unit()
{
    _faction = 0;
    _displayChar = '@';
    _name = "dude";
    _movePoints = 5;
    _maxMovePoints = 5;
    _health = 10;
    _attack = 1;
}

Unit::Unit(int faction, char dchar, std::string name, int move, int maxMove,
           int health, int attack)
{
    _faction = faction;
    _displayChar = dchar;
    _name = name;
    _movePoints = move;
    _maxMovePoints = maxMove;
    _health = health;
    _attack = attack;
}

Unit::~Unit() {}

// Basic getters
int Unit::getFaction() { return _faction; };
char Unit::getChar() { return _displayChar; };
int Unit::getMovePoints() { return _movePoints; };
std::string Unit::getName() { return _name; };
int Unit::getHealth() { return _health; };
int Unit::getAttack() { return _attack; };

// This is called by the similarly named map function on each unit
void Unit::resetMovePoints() { _movePoints = _maxMovePoints; }

// For when the unit takes damage 
void Unit::damageHealth(int dmg)
{
    _health -= dmg;
}

// Gets a bunch of information and returns it as a string to be displayed
// by the game when the unit is selected
std::string Unit::getStatus()
{
    std::string statusline = " ";
    statusline += _displayChar;
    statusline += " " + _name;
    statusline += " hp: ";
    statusline += std::to_string(_health);
    statusline += " mp: " + std::to_string(_movePoints);
    return statusline;
}

// Sets the current move points left as passed in the by map method for moving
// units
void Unit::setMove(int movesLeft)
{
    _movePoints = movesLeft;
}
