#ifndef RENDERER_H
#define RENDERER_H

#include "Map.h"
#include "Tools.h"

class Renderer
{
public:
    Renderer(Map* map, Loc& curs, Rect& frame, Loc& off);
    ~Renderer();
    void init();
    void cleanup();
    void render();
    void refresh();
private:
    Map* _map;
    Rect* _frame;
    Loc* _off;
    Loc* _curs;
};


#endif
