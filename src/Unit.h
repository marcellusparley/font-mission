#ifndef UNIT_H
#define UNIT_H

#include <string>

class Unit
{
public:
    Unit();
    Unit(int faction, char dchar, std::string name, int move, int maxMove,
         int health, int attack);
    ~Unit();
    int getFaction();
    std::string getName();
    int getMovePoints();
    int getHealth();
    int getAttack();
    char getChar();
    void damageHealth(int dmg);
    void setMove(int movesLeft);
    std::string getStatus();
    void resetMovePoints();

private:
    int _faction;
    char _displayChar;
    std::string _name;
    int _movePoints;
    int _maxMovePoints;
    int _health;
    int _attack;
};

#endif
