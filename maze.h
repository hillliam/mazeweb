// Copyright Liam hill 2013
#pragma once
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <emscripten.h>

typedef enum maze_pices {
    north,
    east,
    all,
    eastwastnorth,
    eastwestsouth,
    noenterable,
    southeast,
    southwest,
    northeast,
    northwest
} maze_pices;

class maze {
private:
    bool playerison;
    bool endpoint;
    int distance;
    bool hasbeing;
    bool bestpath;
    bool nonreatchable;
    maze_pices picetype;
public:
    maze(void);
    ~maze(void);
    void print(SDL_Surface *screen, const int x, const int y, const int size, const int linethicness);
    void setplayerison(bool playeron);
    bool getplayerison();
    void setendpoint(bool end);
    bool getendpoint();
    void sethasbeing(bool being);
    bool gethasbeing();
    void setbestpath(bool best);
    bool getbestpath();
    void setnonreatchable(bool notreatch);
    bool getnonreatchable();
    void setdistance(int steps);
    int getdistance();
    maze_pices getpicetype();
    void setpicetype(maze_pices item);
};
