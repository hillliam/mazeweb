#pragma once
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <emscripten.h>

typedef enum maze_pices
{
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

class maze
{
public:
	bool playerison;
  bool endpoint;
	int distance;
	bool hasbeing;
	bool bestpath;
	bool nonreatchable;
  maze_pices picetype;
	maze(void);
	~maze(void);
	void print(const int x, const int y,const int size, const int linethicness);
};
