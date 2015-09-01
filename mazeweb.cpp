// mazeweb.cpp : Defines the entry point for the console application.
// Copyright Liam hill 2013

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "maze.h"
using namespace std;
const int mazesize = 30;
int moves = 0;
int bestmoves = 0;
maze map[mazesize][mazesize];
SDL_Surface *screen;
int with = 820;
int hight = 820;
int bpp = 32;
bool win = false;
int beginx = 0;
int beginy = 0;
int numFrames = 0;
int mousex = 0;
int mousey = 0;
bool ValidMoves(const int startx, const int starty, const int tox, const int toy)
{
    if (tox < 0 || tox > mazesize - 1 || toy < 0 || toy > mazesize - 1)
    {
        return false;
    }
    switch (map[startx][starty].picetype)
    {
        case all:
        case north:
        case eastwastnorth:
        case southeast:
        case southwest:
            if (tox == startx && toy == starty - 1)
            {
                switch (map[tox][toy].picetype)
                {
                    case all:
                    case north:
                    case eastwestsouth:
                    case northeast:
                    case northwest:
                        return true;
                }
            }
            break;
    }
    switch (map[startx][starty].picetype)
    {
        case all:
        case east:
        case eastwestsouth:
        case eastwastnorth:
        case northwest:
        case southwest:
            if (tox == startx - 1 && toy == starty)
            {
                switch (map[tox][toy].picetype)
                {
                    case all:
                    case east:
                    case eastwastnorth:
                    case eastwestsouth:
                    case southeast:
                    case northeast:
                        return true;
                }
            }
            break;
    }
    switch (map[startx][starty].picetype)
    {
        case all:
        case north:
        case eastwestsouth:
        case northeast:
        case northwest:
            if (tox == startx && toy == starty + 1)
            {
                switch (map[tox][toy].picetype)
                {
                    case all:
                    case north:
                    case eastwastnorth:
                    case southeast:
                    case southwest:
                        return true;
                }
            }
            break;
    }
    switch (map[startx][starty].picetype)
    {
        case all:
        case east:
        case eastwestsouth:
        case eastwastnorth:
        case northeast:
        case southeast:
            if (tox == startx + 1 && toy == starty)
            {
                switch (map[tox][toy].picetype)
                {
                    case all:
                    case east:
                    case eastwastnorth:
                    case eastwestsouth:
                    case northwest:
                    case southwest:
                        return true;
                }
            }
            break;
    }
    return false;
}
int getmovesx(int startx, int direction)
{
    switch (direction)
    {
        case 1:
            return startx;
        case 2:
            return startx - 1;
        case 3:
            return startx;
        case 4:
            return startx + 1;
    }
    return 0;
}
int getmovesy(int starty, int direction)
{
    switch (direction)
    {
        case 1:
            return starty - 1;
        case 2:
            return starty;
        case 3:
            return starty + 1;
        case 4:
            return starty;
    }
    return 0;
}
void Pathfind()
{
    int startx = 0;
    int starty = 0;
    bool found = false;
    for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
            if (map[i][j].playerison)
            {
                startx = i;
                starty = j;
                found = true;
                break;
            }
        }
    }
    if (found)
    {
        map[startx][starty].distance = 0;
        while (true)
        {
            bool madeProgress = false;
            for (int i = 0; i < mazesize; i++)
            {
                for (int j = 0; j < mazesize; j++)
                {
                    int x = i;
                    int y = j;
                    int passHere = map[x][y].distance;
                    for (int a = 1; a < 5; a++)
                    {
						int newx = getmovesx(x,a);
						int newy = getmovesy(y,a);
                        if (ValidMoves(x, y, newx, newy))
                        {
                            int newPass = passHere + 1;
                            if (map[newx][newy].distance > newPass)
                            {
                                map[newx][newy].distance = newPass;
                                madeProgress = true;
                            }
                        }
                    }
                }
            }
            if (!madeProgress)
            {
                break;
            }
        }
    }
}
int getbestmoves()
{
	Pathfind();
    for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
            if (map[i][j].endpoint)
            {
                return map[i][j].distance;
            }
        }
    }
    return 0;
}
bool compleatiblemaze()
{
	Pathfind();
	for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
            if (map[i][j].endpoint && map[i][j].distance != 10000)
            {
                return true;
            }
        }
    }
    return false;
}
void rerandommaze()
{
	int lowerBound = 0;
    int upperBound = 30;
    for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
			if (map[i][j].distance == 10000)
			{
				int randnum = (lowerBound + (int)(emscripten_random()* 100) % (10 - lowerBound));
				map[i][j].picetype = (maze_pices)randnum;
				if (map[i][j].picetype == noenterable && map[i][j].playerison)
				{
					map[i][j].picetype = all;
				}
				if (map[i][j].picetype == noenterable && map[i][j].endpoint)
				{
					map[i][j].picetype = all;
				}
			}
        }
    }
}
void showbestpath()
{
	bool found = false;
    int exitx = 0;
    int exity = 0;
    for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
            if (map[i][j].endpoint)
            {
                exitx = i;
                exity = j;
                found = true;
            }
        }
    }
    while (found)
    {
        int lowestx = 0;
        int lowesty = 0;
        int lowest = 10000;
        for (int i = 1; i < 5; i++)
        {
			int newx = getmovesx(exitx,i);
			int newy = getmovesy(exity,i);
            if (ValidMoves(exitx, exity, newx, newy))
            {
                int count = map[newx][newy].distance;
                if (count < lowest)
                {
                    lowest = count;
                    lowestx = newx;
                    lowesty = newy;
                }
            }
        }
        if (lowest != 10000)
        {
            map[lowestx][lowesty].bestpath = true;
            exitx = lowestx;
            exity = lowesty;
        }
        else
        {
            break;
        }
        if (exitx == beginx & exity == beginy)
        {
            break;
        }
    }
}
void shownonreatchable()
{
	for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
            if (map[i][j].distance == 10000)
            {
                map[i][j].nonreatchable = true;
            }
        }
    }
}
void movemaze(const int direction)
{
	int x = 0;
	int y = 0;
	bool canmove = false;
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (map[i][j].playerison)
			{
				x = i;
				y = j;
				canmove = true;
			}
			if (map[i][j].playerison & map[i][j].endpoint)
			{
				canmove = false;
				if (win == false)
				{
					shownonreatchable();
					win = true;
					if (moves > bestmoves)
					{
						EM_ASM_(window.alert('you have compleated the maze in ' + $0 + ' try again and acheave ' + $1),moves, bestmoves);
						cout<<"you have compleated the maze in "<<moves<<" try again and acheave "<<bestmoves<<endl;
						showbestpath();
					}
					else
					{
						EM_ASM_(window.alert('you have compleated the maze in ' + $0),moves);
						cout<<"you have compleated the maze in "<<moves<<endl;
					}
				}
				else
				{
					EM_ASM_(window.alert('you have compleated the maze in ' + $0 + ' try a new maze'),moves);
					cout<<"you have compleated the maze in "<<moves<<" try a new maze"<<endl;
				}
			}
		}
	}
	if (canmove)
	{
		if (direction == 1)// up
		{
			if (y != 0)
			{
				switch (map[x][y].picetype)
				{
					case all:
					case north:
					case eastwastnorth:
					case southeast:
					case southwest:
						switch(map[x][y - 1].picetype)
						{
							case all:
							case north:
							case eastwestsouth:
							case northeast:
							case northwest:
								map[x][y].playerison = false;
								map[x][y - 1].playerison = true;
								moves++;
							break;
						}
					break;
				}

			}
		}
		if (direction == 2)//left
		{
			if (x != 0)
			{
				switch (map[x][y].picetype)
				{
					case all:
					case east:
					case eastwestsouth:
					case eastwastnorth:
					case northwest:
					case southwest:
						switch (map[x - 1][y].picetype)
						{
							case all:
							case east:
							case eastwastnorth:
							case eastwestsouth:
							case southeast:
							case northeast:
								map[x][y].playerison = false;
								map[x - 1][y].playerison = true;
								moves++;
							break;
						}
					break;
				}
			}
		}
		if (direction == 3)//down
		{
			if (y != 30 - 1)
			{
				switch (map[x][y].picetype)
				{
					case all:
					case north:
					case eastwestsouth:
					case northeast:
					case northwest:
						switch (map[x][y + 1].picetype)
						{
							case all:
							case north:
							case eastwastnorth:
							case southeast:
							case southwest:
								map[x][y].playerison = false;
								map[x][y + 1].playerison = true;
								moves++;
								break;
						}
					break;
				}
			}
		}
		if (direction == 4)//right
		{
			if (x != 30 - 1)
			{
				switch (map[x][y].picetype)
				{
					case all:
					case east:
					case eastwestsouth:
					case eastwastnorth:
					case northeast:
					case southeast:
						switch (map[x + 1][y].picetype)
						{
							case all:
							case east:
							case eastwastnorth:
							case eastwestsouth:
							case northwest:
							case southwest:
								map[x][y].playerison = false;
								map[x + 1][y].playerison = true;
								moves++;
								break;
						}
					break;
				}
			}
		}
	}
}
void drawmaze()
{
	//SDL_LockSurface(screen);
	int space = 25;
    int linethicness = 5;
    int locationx = 20;
    int locationy = 30;
    for (int i = 0; i < mazesize; i++)
    {
        locationy = 30;
        for (int j = 0; j < mazesize; j++)
        {
            locationy += space;
            map[i][j].print(locationx ,locationy,space,linethicness);
			if (map[i][j].endpoint && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space)
			{
				cout<<"this is the location you have to reach"<<endl;
			}
			else if (map[i][j].playerison && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space)
			{
				cout<<"you are here"<<endl;
			}
			else if (map[i][j].bestpath && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space)
			{
				cout<<"the way you should have gone"<<endl;
			}
			else if (map[i][j].hasbeing && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space)
			{
				cout<<"you where here"<<endl;
			}
			else if (map[i][j].nonreatchable && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space)
			{
				cout<<"not reatchable"<<endl;
			}

        }
        locationx += space;
    }
	SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, with, hight);
}
void randommaze()
{
	int lowerBound = 0;
    int upperBound = 30;
    int rndValue1 = (lowerBound + (int)(emscripten_random()* 100) % (upperBound - lowerBound));
    int rndValue2 = (lowerBound + (int)(emscripten_random()* 100) % (upperBound - lowerBound));
    for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
            map[i][j].playerison = false;
            map[i][j].endpoint = false;
        }
    }
    map[rndValue1][rndValue2].playerison = true;
	rndValue1 = (lowerBound + (int)(emscripten_random()* 100) % (upperBound - lowerBound));
	rndValue2 = (lowerBound + (int)(emscripten_random()* 100) % (upperBound - lowerBound));
    map[rndValue1][rndValue2].endpoint = true;
    for (int i = 0; i < mazesize; i++)
    {
        for (int j = 0; j < mazesize; j++)
        {
			int randnum = (lowerBound + (int)(emscripten_random()* 100) % (10 - lowerBound));
            map[i][j].picetype = (maze_pices)randnum;
			if (map[i][j].picetype == noenterable && map[i][j].playerison)
			{
				map[i][j].picetype = all;
			}
			if (map[i][j].picetype == noenterable && map[i][j].endpoint)
			{
				map[i][j].picetype = all;
			}
        }
    }
}
void getkinput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
			case SDLK_RIGHT:
			case SDLK_d:
				movemaze(4);
				break;
			case SDLK_LEFT:
			case SDLK_a:
				movemaze(2);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				movemaze(3);
				break;
			case SDLK_UP:
			case SDLK_w:
				movemaze(1);
			    break;
		}
	}
}
void getminput()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	mousex = x;
	mousey = y;
}
void geteditminput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	int x, y;
	SDL_GetMouseState(&x, &y);
	if( event.type == SDL_MOUSEBUTTONDOWN )
    {
		int space = 25;
        int linethicness = 5;
        int locationx = 20;
        int locationy = 30;
        for (int i = 0; i < mazesize; i++)
        {
            locationy = 30;
            for (int j = 0; j < mazesize; j++)
            {
                locationy += space;
				if (x >= locationx && x <= locationx + space && y >= locationy && y <= locationy + space)
				{
					int action = EM_ASM_INT_V({var radios = document.getElementsByName('mapset');
					for (var i = 0, length = radios.length; i < length; i++) {
						if (radios[i].checked)
						{
							return i;
						}
					}
					});
					map[i][j].picetype = (maze_pices)action;
					cout<<"changed section "<<i<<" "<<j<<" to "<<((maze_pices)action)<<"at "<<x<<" and"<<y<<endl;
					break;
				}
            }
            locationx += space;
        }
	}
}
void looplogic()
{
	getkinput();
	getminput();
	drawmaze();
	//numFrames++;
	//float fps = ( numFrames/(float)(SDL_GetTicks()) )*1000;
	//cout<<"the frames per second is at : "<< fps<<" and the total number of frames is : "<<numFrames<<endl;
}
void editloop()
{
	geteditminput();
	drawmaze();
}
extern "C"
{
	void clearmap()
	{
		for (int i = 0; i < mazesize; i++)
		{
			for (int j = 0; j < mazesize; j++)
            {
				    map[i][j].playerison = false;
                    map[i][j].endpoint = false;
					map[i][j].bestpath = false;
					map[i][j].hasbeing = false;
					map[i][j].nonreatchable = false;
					map[i][j].distance = 10000;
					map[i][j].picetype = all;
			}
		}
	}
	void loadmap(vector<string> data)
	{
		int r = 0;
		for (int i = 0; i < mazesize; i++)
		{
			for (int j = 0; j < mazesize;j++)
			{
				if (data[r] == "north")
				{
					map[i][j].picetype = north;
				}
				if (data[r] == "east")
				{
					map[i][j].picetype = east;
				}
				if (data[r] == "all")
				{
					map[i][j].picetype = north;
				}
				if (data[r] == "eastwastnorth")
				{
					map[i][j].picetype = eastwastnorth;
				}
				if (data[r] == "eastwestsouth")
				{
					map[i][j].picetype = eastwestsouth;
				}
				if (data[r] == "noenterable")
				{
					map[i][j].picetype = noenterable;
				}
				if (data[r] == "southeast")
				{
					map[i][j].picetype = southeast;
				}
				if (data[r] == "southwest")
				{
					map[i][j].picetype = southwest;
				}
				if (data[r] == "northeast")
				{
					map[i][j].picetype = northeast;
				}
				if (data[r] == "northwest")
				{
					map[i][j].picetype = northwest;
				}
				r++;
				if (data[r] == "true" || data[r] == "TRUE")
				{
					map[i][j].playerison = true;
				}
				else
				{
					map[i][j].playerison = false;
				}
				r++;
				if (data[r] == "true" || data[r] == "TRUE")
				{
					map[i][j].endpoint = true;
				}
				else
				{
					map[i][j].endpoint = false;
				}
				r++;
			}
		}
		moves = 0;
		cout<<"begin"<<endl;
		SDL_Init(SDL_INIT_VIDEO);
		atexit(SDL_Quit);
		screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
		if ( screen == NULL )
		{
			cout<<"no canvas"<<endl;
		}
		drawmaze();
		cout<<"start"<<endl;
		//startTime = SDL_GetTicks();
		emscripten_set_main_loop(looplogic,0,0);
	}
	void loadfile(string file)
	{
		clearmap();
		cout<<file<<endl;
		vector<string> v;
	    unsigned int pos = file.find(';');
        unsigned int initialPos = 0;
        v.clear();
        while( pos != string::npos ) 
	    {
			v.push_back( file.substr( initialPos, pos - initialPos + 1 ) );
			initialPos = pos + 1;
			pos = file.find( ';', initialPos );
		}

    // Add the last one
		v.push_back( file.substr( initialPos, min( pos, file.size() ) - initialPos + 1 ) );
	    loadmap(v);
	}
	void loadtext()
	{
		clearmap();
	    string file;
	    cin>>file;
	    vector<string> v;
	    unsigned int pos = file.find(';');
        unsigned int initialPos = 0;
        v.clear();
        while( pos != string::npos ) 
		{
			v.push_back( file.substr( initialPos, pos - initialPos + 1 ) );
            initialPos = pos + 1;
            pos = file.find( ';', initialPos );
		}

    // Add the last one
		v.push_back( file.substr( initialPos, min( pos, file.size() ) - initialPos + 1 ) );
	    loadmap(v);
	}
	int startall()
	{
		clearmap();
		cout<<"begin"<<endl;
		SDL_Init(SDL_INIT_VIDEO);
		atexit(SDL_Quit);
		screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
		if ( screen == NULL )
		{
			cout<<"no canvas"<<endl;
		}
		randommaze();
		drawmaze();
		cout<<"start"<<endl;
		bestmoves = getbestmoves();
		for (int i = 0; i < mazesize; i++)
		{
			for (int j = 0; j < mazesize; j++)
			{
				if (map[i][j].playerison)
				{
					beginx = i;
					beginy = j;
				}
			}
		}
		//startTime = SDL_GetTicks();
		emscripten_set_main_loop(looplogic,0,0);
		return 0;
	}
	int blankstart()
	{
		cout<<"begin"<<endl;
		SDL_Init(SDL_INIT_VIDEO);
		atexit(SDL_Quit);
		screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
		if ( screen == NULL )
		{
			cout<<"no canvas"<<endl;
		}
		drawmaze();
		cout<<"start"<<endl;
		bestmoves = getbestmoves();
		for (int i = 0; i < mazesize; i++)
		{
			for (int j = 0; j < mazesize; j++)
			{
				if (map[i][j].playerison)
				{
					beginx = i;
					beginy = j;
				}
			}
		}
		//startTime = SDL_GetTicks();
		emscripten_set_main_loop(looplogic,0,0);
		return 0;
	}
	void editmode()
	{
		clearmap();
		cout<<"begin"<<endl;
		SDL_Init(SDL_INIT_VIDEO);
		atexit(SDL_Quit);
		screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
		if ( screen == NULL )
		{
			cout<<"no canvas"<<endl;
		}
		drawmaze();
		cout<<"start"<<endl;
		emscripten_set_main_loop(editloop,0,0);
	}
	void preset1()
	{
		clearmap();
		map[0][0].picetype = north;
		map[0][0].playerison = true;
		map[0][0].endpoint = false;
		map[0][1].picetype = north;
		map[0][1].playerison = false;
		map[0][1].endpoint = false;
		map[0][2].picetype = north;
		map[0][2].playerison = false;
		map[0][2].endpoint = false;
		map[0][3].picetype = north;
		map[0][3].playerison = false;
		map[0][3].endpoint = false;
		map[0][4].picetype = north;
		map[0][4].playerison = false;
		map[0][4].endpoint = false;
		map[0][5].picetype = north;
		map[0][5].playerison = false;
		map[0][5].endpoint = false;
		map[0][6].picetype = north;
		map[0][6].playerison = false;
		map[0][6].endpoint = false;
		map[0][7].picetype = north;
		map[0][7].playerison = false;
		map[0][7].endpoint = false;
		map[0][8].picetype = north;
		map[0][8].playerison = false;
		map[0][8].endpoint = false;
		map[0][9].picetype = north;
		map[0][9].playerison = false;
		map[0][9].endpoint = false;
		map[0][10].picetype = north;
		map[0][10].playerison = false;
		map[0][10].endpoint = false;
		map[0][11].picetype = north;
		map[0][11].playerison = false;
		map[0][11].endpoint = false;
		map[0][12].picetype = north;
		map[0][12].playerison = false;
		map[0][12].endpoint = false;
		map[0][13].picetype = north;
		map[0][13].playerison = false;
		map[0][13].endpoint = false;
		map[0][14].picetype = north;
		map[0][14].playerison = false;
		map[0][14].endpoint = false;
		map[0][15].picetype = north;
		map[0][15].playerison = false;
		map[0][15].endpoint = false;
		map[0][16].picetype = north;
		map[0][16].playerison = false;
		map[0][16].endpoint = false;
		map[0][17].picetype = north;
		map[0][17].playerison = false;
		map[0][17].endpoint = false;
		map[0][18].picetype = north;
		map[0][18].playerison = false;
		map[0][18].endpoint = false;
		map[0][19].picetype = north;
		map[0][19].playerison = false;
		map[0][19].endpoint = false;
		map[0][20].picetype = north;
		map[0][20].playerison = false;
		map[0][20].endpoint = false;
		map[0][21].picetype = north;
		map[0][21].playerison = false;
		map[0][21].endpoint = false;
		map[0][22].picetype = north;
		map[0][22].playerison = false;
		map[0][22].endpoint = false;
		map[0][23].picetype = north;
		map[0][23].playerison = false;
		map[0][23].endpoint = false;
		map[0][24].picetype = north;
		map[0][24].playerison = false;
		map[0][24].endpoint = false;
		map[0][25].picetype = north;
		map[0][25].playerison = false;
		map[0][25].endpoint = false;
		map[0][26].picetype = north;
		map[0][26].playerison = false;
		map[0][26].endpoint = false;
		map[0][27].picetype = north;
		map[0][27].playerison = false;
		map[0][27].endpoint = false;
		map[0][28].picetype = north;
		map[0][28].playerison = false;
		map[0][28].endpoint = false;
		map[0][29].picetype = southeast;
		map[0][29].playerison = false;
		map[0][29].endpoint = false;
		map[1][0].picetype = northeast;
		map[1][0].playerison = false;
		map[1][0].endpoint = false;
		map[1][1].picetype = north;
		map[1][1].playerison = false;
		map[1][1].endpoint = false;
		map[1][2].picetype = north;
		map[1][2].playerison = false;
		map[1][2].endpoint = false;
		map[1][3].picetype = north;
		map[1][3].playerison = false;
		map[1][3].endpoint = false;
		map[1][4].picetype = north;
		map[1][4].playerison = false;
		map[1][4].endpoint = false;
		map[1][5].picetype = north;
		map[1][5].playerison = false;
		map[1][5].endpoint = false;
		map[1][6].picetype = north;
		map[1][6].playerison = false;
		map[1][6].endpoint = false;
		map[1][7].picetype = north;
		map[1][7].playerison = false;
		map[1][7].endpoint = false;
		map[1][8].picetype = north;
		map[1][8].playerison = false;
		map[1][8].endpoint = false;
		map[1][9].picetype = north;
		map[1][9].playerison = false;
		map[1][9].endpoint = false;
		map[1][10].picetype = north;
		map[1][10].playerison = false;
		map[1][10].endpoint = false;
		map[1][11].picetype = north;
		map[1][11].playerison = false;
		map[1][11].endpoint = false;
		map[1][12].picetype = north;
		map[1][12].playerison = false;
		map[1][12].endpoint = false;
		map[1][13].picetype = north;
		map[1][13].playerison = false;
		map[1][13].endpoint = false;
		map[1][14].picetype = north;
		map[1][14].playerison = false;
		map[1][14].endpoint = false;
		map[1][15].picetype = north;
		map[1][15].playerison = false;
		map[1][15].endpoint = false;
		map[1][16].picetype = north;
		map[1][16].playerison = false;
		map[1][16].endpoint = false;
		map[1][17].picetype = north;
		map[1][17].playerison = false;
		map[1][17].endpoint = false;
		map[1][18].picetype = north;
		map[1][18].playerison = false;
		map[1][18].endpoint = false;
		map[1][19].picetype = north;
		map[1][19].playerison = false;
		map[1][19].endpoint = false;
		map[1][20].picetype = north;
		map[1][20].playerison = false;
		map[1][20].endpoint = false;
		map[1][21].picetype = north;
		map[1][21].playerison = false;
		map[1][21].endpoint = false;
		map[1][22].picetype = north;
		map[1][22].playerison = false;
		map[1][22].endpoint = false;
		map[1][23].picetype = north;
		map[1][23].playerison = false;
		map[1][23].endpoint = false;
		map[1][24].picetype = north;
		map[1][24].playerison = false;
		map[1][24].endpoint = false;
		map[1][25].picetype = north;
		map[1][25].playerison = false;
		map[1][25].endpoint = false;
		map[1][26].picetype = north;
		map[1][26].playerison = false;
		map[1][26].endpoint = false;
		map[1][27].picetype = north;
		map[1][27].playerison = false;
		map[1][27].endpoint = false;
		map[1][28].picetype = north;
		map[1][28].playerison = false;
		map[1][28].endpoint = false;
		map[1][29].picetype = southwest;
		map[1][29].playerison = false;
		map[1][29].endpoint = false;
		map[2][0].picetype = northwest;
		map[2][0].playerison = false;
		map[2][0].endpoint = false;
		map[2][1].picetype = north;
		map[2][1].playerison = false;
		map[2][1].endpoint = false;
		map[2][2].picetype = north;
		map[2][2].playerison = false;
		map[2][2].endpoint = false;
		map[2][3].picetype = north;
		map[2][3].playerison = false;
		map[2][3].endpoint = false;
		map[2][4].picetype = north;
		map[2][4].playerison = false;
		map[2][4].endpoint = false;
		map[2][5].picetype = north;
		map[2][5].playerison = false;
		map[2][5].endpoint = false;
		map[2][6].picetype = north;
		map[2][6].playerison = false;
		map[2][6].endpoint = false;
		map[2][7].picetype = north;
		map[2][7].playerison = false;
		map[2][7].endpoint = false;
		map[2][8].picetype = north;
		map[2][8].playerison = false;
		map[2][8].endpoint = false;
		map[2][9].picetype = north;
		map[2][9].playerison = false;
		map[2][9].endpoint = false;
		map[2][10].picetype = north;
		map[2][10].playerison = false;
		map[2][10].endpoint = false;
		map[2][11].picetype = north;
		map[2][11].playerison = false;
		map[2][11].endpoint = false;
		map[2][12].picetype = north;
		map[2][12].playerison = false;
		map[2][12].endpoint = false;
		map[2][13].picetype = north;
		map[2][13].playerison = false;
		map[2][13].endpoint = false;
		map[2][14].picetype = north;
		map[2][14].playerison = false;
		map[2][14].endpoint = false;
		map[2][15].picetype = north;
		map[2][15].playerison = false;
		map[2][15].endpoint = false;
		map[2][16].picetype = north;
		map[2][16].playerison = false;
		map[2][16].endpoint = false;
		map[2][17].picetype = north;
		map[2][17].playerison = false;
		map[2][17].endpoint = false;
		map[2][18].picetype = north;
		map[2][18].playerison = false;
		map[2][18].endpoint = false;
		map[2][19].picetype = north;
		map[2][19].playerison = false;
		map[2][19].endpoint = false;
		map[2][20].picetype = north;
		map[2][20].playerison = false;
		map[2][20].endpoint = false;
		map[2][21].picetype = north;
		map[2][21].playerison = false;
		map[2][21].endpoint = false;
		map[2][22].picetype = north;
		map[2][22].playerison = false;
		map[2][22].endpoint = false;
		map[2][23].picetype = north;
		map[2][23].playerison = false;
		map[2][23].endpoint = false;
		map[2][24].picetype = north;
		map[2][24].playerison = false;
		map[2][24].endpoint = false;
		map[2][25].picetype = north;
		map[2][25].playerison = false;
		map[2][25].endpoint = false;
		map[2][26].picetype = north;
		map[2][26].playerison = false;
		map[2][26].endpoint = false;
		map[2][27].picetype = north;
		map[2][27].playerison = false;
		map[2][27].endpoint = false;
		map[2][28].picetype = north;
		map[2][28].playerison = false;
		map[2][28].endpoint = false;
		map[2][29].picetype = southeast;
		map[2][29].playerison = false;
		map[2][29].endpoint = false;
		map[3][0].picetype = northeast;
		map[3][0].playerison = false;
		map[3][0].endpoint = false;
		map[3][1].picetype = north;
		map[3][1].playerison = false;
		map[3][1].endpoint = false;
		map[3][2].picetype = north;
		map[3][2].playerison = false;
		map[3][2].endpoint = false;
		map[3][3].picetype = north;
		map[3][3].playerison = false;
		map[3][3].endpoint = false;
		map[3][4].picetype = north;
		map[3][4].playerison = false;
		map[3][4].endpoint = false;
		map[3][5].picetype = north;
		map[3][5].playerison = false;
		map[3][5].endpoint = false;
		map[3][6].picetype = north;
		map[3][6].playerison = false;
		map[3][6].endpoint = false;
		map[3][7].picetype = north;
		map[3][7].playerison = false;
		map[3][7].endpoint = false;
		map[3][8].picetype = north;
		map[3][8].playerison = false;
		map[3][8].endpoint = false;
		map[3][9].picetype = north;
		map[3][9].playerison = false;
		map[3][9].endpoint = false;
		map[3][10].picetype = north;
		map[3][10].playerison = false;
		map[3][10].endpoint = false;
		map[3][11].picetype = north;
		map[3][11].playerison = false;
		map[3][11].endpoint = false;
		map[3][12].picetype = north;
		map[3][12].playerison = false;
		map[3][12].endpoint = false;
		map[3][13].picetype = north;
		map[3][13].playerison = false;
		map[3][13].endpoint = false;
		map[3][14].picetype = north;
		map[3][14].playerison = false;
		map[3][14].endpoint = false;
		map[3][15].picetype = north;
		map[3][15].playerison = false;
		map[3][15].endpoint = false;
		map[3][16].picetype = north;
		map[3][16].playerison = false;
		map[3][16].endpoint = false;
		map[3][17].picetype = north;
		map[3][17].playerison = false;
		map[3][17].endpoint = false;
		map[3][18].picetype = north;
		map[3][18].playerison = false;
		map[3][18].endpoint = false;
		map[3][19].picetype = north;
		map[3][19].playerison = false;
		map[3][19].endpoint = false;
		map[3][20].picetype = north;
		map[3][20].playerison = false;
		map[3][20].endpoint = false;
		map[3][21].picetype = north;
		map[3][21].playerison = false;
		map[3][21].endpoint = false;
		map[3][22].picetype = north;
		map[3][22].playerison = false;
		map[3][22].endpoint = false;
		map[3][23].picetype = north;
		map[3][23].playerison = false;
		map[3][23].endpoint = false;
		map[3][24].picetype = north;
		map[3][24].playerison = false;
		map[3][24].endpoint = false;
		map[3][25].picetype = north;
		map[3][25].playerison = false;
		map[3][25].endpoint = false;
		map[3][26].picetype = north;
		map[3][26].playerison = false;
		map[3][26].endpoint = false;
		map[3][27].picetype = north;
		map[3][27].playerison = false;
		map[3][27].endpoint = false;
		map[3][28].picetype = north;
		map[3][28].playerison = false;
		map[3][28].endpoint = false;
		map[3][29].picetype = southwest;
		map[3][29].playerison = false;
		map[3][29].endpoint = false;
		map[4][0].picetype = northwest;
		map[4][0].playerison = false;
		map[4][0].endpoint = false;
		map[4][1].picetype = north;
		map[4][1].playerison = false;
		map[4][1].endpoint = false;
		map[4][2].picetype = north;
		map[4][2].playerison = false;
		map[4][2].endpoint = false;
		map[4][3].picetype = north;
		map[4][3].playerison = false;
		map[4][3].endpoint = false;
		map[4][4].picetype = north;
		map[4][4].playerison = false;
		map[4][4].endpoint = false;
		map[4][5].picetype = north;
		map[4][5].playerison = false;
		map[4][5].endpoint = false;
		map[4][6].picetype = north;
		map[4][6].playerison = false;
		map[4][6].endpoint = false;
		map[4][7].picetype = north;
		map[4][7].playerison = false;
		map[4][7].endpoint = false;
		map[4][8].picetype = north;
		map[4][8].playerison = false;
		map[4][8].endpoint = false;
		map[4][9].picetype = north;
		map[4][9].playerison = false;
		map[4][9].endpoint = false;
		map[4][10].picetype = north;
		map[4][10].playerison = false;
		map[4][10].endpoint = false;
		map[4][11].picetype = north;
		map[4][11].playerison = false;
		map[4][11].endpoint = false;
		map[4][12].picetype = north;
		map[4][12].playerison = false;
		map[4][12].endpoint = false;
		map[4][13].picetype = north;
		map[4][13].playerison = false;
		map[4][13].endpoint = false;
		map[4][14].picetype = north;
		map[4][14].playerison = false;
		map[4][14].endpoint = false;
		map[4][15].picetype = north;
		map[4][15].playerison = false;
		map[4][15].endpoint = false;
		map[4][16].picetype = north;
		map[4][16].playerison = false;
		map[4][16].endpoint = false;
		map[4][17].picetype = north;
		map[4][17].playerison = false;
		map[4][17].endpoint = false;
		map[4][18].picetype = north;
		map[4][18].playerison = false;
		map[4][18].endpoint = false;
		map[4][19].picetype = north;
		map[4][19].playerison = false;
		map[4][19].endpoint = false;
		map[4][20].picetype = north;
		map[4][20].playerison = false;
		map[4][20].endpoint = false;
		map[4][21].picetype = north;
		map[4][21].playerison = false;
		map[4][21].endpoint = false;
		map[4][22].picetype = north;
		map[4][22].playerison = false;
		map[4][22].endpoint = false;
		map[4][23].picetype = north;
		map[4][23].playerison = false;
		map[4][23].endpoint = false;
		map[4][24].picetype = north;
		map[4][24].playerison = false;
		map[4][24].endpoint = false;
		map[4][25].picetype = north;
		map[4][25].playerison = false;
		map[4][25].endpoint = false;
		map[4][26].picetype = north;
		map[4][26].playerison = false;
		map[4][26].endpoint = false;
		map[4][27].picetype = north;
		map[4][27].playerison = false;
		map[4][27].endpoint = false;
		map[4][28].picetype = north;
		map[4][28].playerison = false;
		map[4][28].endpoint = false;
		map[4][29].picetype = southeast;
		map[4][29].playerison = false;
		map[4][29].endpoint = false;
		map[5][0].picetype = northeast;
		map[5][0].playerison = false;
		map[5][0].endpoint = false;
		map[5][1].picetype = north;
		map[5][1].playerison = false;
		map[5][1].endpoint = false;
		map[5][2].picetype = north;
		map[5][2].playerison = false;
		map[5][2].endpoint = false;
		map[5][3].picetype = north;
		map[5][3].playerison = false;
		map[5][3].endpoint = false;
		map[5][4].picetype = north;
		map[5][4].playerison = false;
		map[5][4].endpoint = false;
		map[5][5].picetype = north;
		map[5][5].playerison = false;
		map[5][5].endpoint = false;
		map[5][6].picetype = north;
		map[5][6].playerison = false;
		map[5][6].endpoint = false;
		map[5][7].picetype = north;
		map[5][7].playerison = false;
		map[5][7].endpoint = false;
		map[5][8].picetype = north;
		map[5][8].playerison = false;
		map[5][8].endpoint = false;
		map[5][9].picetype = north;
		map[5][9].playerison = false;
		map[5][9].endpoint = false;
		map[5][10].picetype = north;
		map[5][10].playerison = false;
		map[5][10].endpoint = false;
		map[5][11].picetype = north;
		map[5][11].playerison = false;
		map[5][11].endpoint = false;
		map[5][12].picetype = north;
		map[5][12].playerison = false;
		map[5][12].endpoint = false;
		map[5][13].picetype = north;
		map[5][13].playerison = false;
		map[5][13].endpoint = false;
		map[5][14].picetype = north;
		map[5][14].playerison = false;
		map[5][14].endpoint = false;
		map[5][15].picetype = north;
		map[5][15].playerison = false;
		map[5][15].endpoint = false;
		map[5][16].picetype = north;
		map[5][16].playerison = false;
		map[5][16].endpoint = false;
		map[5][17].picetype = north;
		map[5][17].playerison = false;
		map[5][17].endpoint = false;
		map[5][18].picetype = north;
		map[5][18].playerison = false;
		map[5][18].endpoint = false;
		map[5][19].picetype = north;
		map[5][19].playerison = false;
		map[5][19].endpoint = false;
		map[5][20].picetype = north;
		map[5][20].playerison = false;
		map[5][20].endpoint = false;
		map[5][21].picetype = north;
		map[5][21].playerison = false;
		map[5][21].endpoint = false;
		map[5][22].picetype = north;
		map[5][22].playerison = false;
		map[5][22].endpoint = false;
		map[5][23].picetype = north;
		map[5][23].playerison = false;
		map[5][23].endpoint = false;
		map[5][24].picetype = north;
		map[5][24].playerison = false;
		map[5][24].endpoint = false;
		map[5][25].picetype = north;
		map[5][25].playerison = false;
		map[5][25].endpoint = false;
		map[5][26].picetype = north;
		map[5][26].playerison = false;
		map[5][26].endpoint = false;
		map[5][27].picetype = north;
		map[5][27].playerison = false;
		map[5][27].endpoint = false;
		map[5][28].picetype = north;
		map[5][28].playerison = false;
		map[5][28].endpoint = false;
		map[5][29].picetype = southwest;
		map[5][29].playerison = false;
		map[5][29].endpoint = false;
		map[6][0].picetype = northwest;
		map[6][0].playerison = false;
		map[6][0].endpoint = false;
		map[6][1].picetype = north;
		map[6][1].playerison = false;
		map[6][1].endpoint = false;
		map[6][2].picetype = north;
		map[6][2].playerison = false;
		map[6][2].endpoint = false;
		map[6][3].picetype = north;
		map[6][3].playerison = false;
		map[6][3].endpoint = false;
		map[6][4].picetype = north;
		map[6][4].playerison = false;
		map[6][4].endpoint = false;
		map[6][5].picetype = north;
		map[6][5].playerison = false;
		map[6][5].endpoint = false;
		map[6][6].picetype = north;
		map[6][6].playerison = false;
		map[6][6].endpoint = false;
		map[6][7].picetype = north;
		map[6][7].playerison = false;
		map[6][7].endpoint = false;
		map[6][8].picetype = north;
		map[6][8].playerison = false;
		map[6][8].endpoint = false;
		map[6][9].picetype = north;
		map[6][9].playerison = false;
		map[6][9].endpoint = false;
		map[6][10].picetype = north;
		map[6][10].playerison = false;
		map[6][10].endpoint = false;
		map[6][11].picetype = north;
		map[6][11].playerison = false;
		map[6][11].endpoint = false;
		map[6][12].picetype = north;
		map[6][12].playerison = false;
		map[6][12].endpoint = false;
		map[6][13].picetype = north;
		map[6][13].playerison = false;
		map[6][13].endpoint = false;
		map[6][14].picetype = north;
		map[6][14].playerison = false;
		map[6][14].endpoint = false;
		map[6][15].picetype = north;
		map[6][15].playerison = false;
		map[6][15].endpoint = false;
		map[6][16].picetype = north;
		map[6][16].playerison = false;
		map[6][16].endpoint = false;
		map[6][17].picetype = north;
		map[6][17].playerison = false;
		map[6][17].endpoint = false;
		map[6][18].picetype = north;
		map[6][18].playerison = false;
		map[6][18].endpoint = false;
		map[6][19].picetype = north;
		map[6][19].playerison = false;
		map[6][19].endpoint = false;
		map[6][20].picetype = north;
		map[6][20].playerison = false;
		map[6][20].endpoint = false;
		map[6][21].picetype = north;
		map[6][21].playerison = false;
		map[6][21].endpoint = false;
		map[6][22].picetype = north;
		map[6][22].playerison = false;
		map[6][22].endpoint = false;
		map[6][23].picetype = north;
		map[6][23].playerison = false;
		map[6][23].endpoint = false;
		map[6][24].picetype = north;
		map[6][24].playerison = false;
		map[6][24].endpoint = false;
		map[6][25].picetype = north;
		map[6][25].playerison = false;
		map[6][25].endpoint = false;
		map[6][26].picetype = north;
		map[6][26].playerison = false;
		map[6][26].endpoint = false;
		map[6][27].picetype = north;
		map[6][27].playerison = false;
		map[6][27].endpoint = false;
		map[6][28].picetype = north;
		map[6][28].playerison = false;
		map[6][28].endpoint = false;
		map[6][29].picetype = southeast;
		map[6][29].playerison = false;
		map[6][29].endpoint = false;
		map[7][0].picetype = northeast;
		map[7][0].playerison = false;
		map[7][0].endpoint = false;
		map[7][1].picetype = north;
		map[7][1].playerison = false;
		map[7][1].endpoint = false;
		map[7][2].picetype = north;
		map[7][2].playerison = false;
		map[7][2].endpoint = false;
		map[7][3].picetype = north;
		map[7][3].playerison = false;
		map[7][3].endpoint = false;
		map[7][4].picetype = north;
		map[7][4].playerison = false;
		map[7][4].endpoint = false;
		map[7][5].picetype = north;
		map[7][5].playerison = false;
		map[7][5].endpoint = false;
		map[7][6].picetype = north;
		map[7][6].playerison = false;
		map[7][6].endpoint = false;
		map[7][7].picetype = north;
		map[7][7].playerison = false;
		map[7][7].endpoint = false;
		map[7][8].picetype = north;
		map[7][8].playerison = false;
		map[7][8].endpoint = false;
		map[7][9].picetype = north;
		map[7][9].playerison = false;
		map[7][9].endpoint = false;
		map[7][10].picetype = north;
		map[7][10].playerison = false;
		map[7][10].endpoint = false;
		map[7][11].picetype = north;
		map[7][11].playerison = false;
		map[7][11].endpoint = false;
		map[7][12].picetype = north;
		map[7][12].playerison = false;
		map[7][12].endpoint = false;
		map[7][13].picetype = north;
		map[7][13].playerison = false;
		map[7][13].endpoint = false;
		map[7][14].picetype = north;
		map[7][14].playerison = false;
		map[7][14].endpoint = false;
		map[7][15].picetype = north;
		map[7][15].playerison = false;
		map[7][15].endpoint = false;
		map[7][16].picetype = north;
		map[7][16].playerison = false;
		map[7][16].endpoint = false;
		map[7][17].picetype = north;
		map[7][17].playerison = false;
		map[7][17].endpoint = false;
		map[7][18].picetype = north;
		map[7][18].playerison = false;
		map[7][18].endpoint = false;
		map[7][19].picetype = north;
		map[7][19].playerison = false;
		map[7][19].endpoint = false;
		map[7][20].picetype = north;
		map[7][20].playerison = false;
		map[7][20].endpoint = false;
		map[7][21].picetype = north;
		map[7][21].playerison = false;
		map[7][21].endpoint = false;
		map[7][22].picetype = north;
		map[7][22].playerison = false;
		map[7][22].endpoint = false;
		map[7][23].picetype = north;
		map[7][23].playerison = false;
		map[7][23].endpoint = false;
		map[7][24].picetype = north;
		map[7][24].playerison = false;
		map[7][24].endpoint = false;
		map[7][25].picetype = north;
		map[7][25].playerison = false;
		map[7][25].endpoint = false;
		map[7][26].picetype = north;
		map[7][26].playerison = false;
		map[7][26].endpoint = false;
		map[7][27].picetype = north;
		map[7][27].playerison = false;
		map[7][27].endpoint = false;
		map[7][28].picetype = north;
		map[7][28].playerison = false;
		map[7][28].endpoint = false;
		map[7][29].picetype = southwest;
		map[7][29].playerison = false;
		map[7][29].endpoint = false;
		map[8][0].picetype = northwest;
		map[8][0].playerison = false;
		map[8][0].endpoint = false;
		map[8][1].picetype = north;
		map[8][1].playerison = false;
		map[8][1].endpoint = false;
		map[8][2].picetype = north;
		map[8][2].playerison = false;
		map[8][2].endpoint = false;
		map[8][3].picetype = north;
		map[8][3].playerison = false;
		map[8][3].endpoint = false;
		map[8][4].picetype = north;
		map[8][4].playerison = false;
		map[8][4].endpoint = false;
		map[8][5].picetype = north;
		map[8][5].playerison = false;
		map[8][5].endpoint = false;
		map[8][6].picetype = north;
		map[8][6].playerison = false;
		map[8][6].endpoint = false;
		map[8][7].picetype = north;
		map[8][7].playerison = false;
		map[8][7].endpoint = false;
		map[8][8].picetype = north;
		map[8][8].playerison = false;
		map[8][8].endpoint = false;
		map[8][9].picetype = north;
		map[8][9].playerison = false;
		map[8][9].endpoint = false;
		map[8][10].picetype = north;
		map[8][10].playerison = false;
		map[8][10].endpoint = false;
		map[8][11].picetype = north;
		map[8][11].playerison = false;
		map[8][11].endpoint = false;
		map[8][12].picetype = north;
		map[8][12].playerison = false;
		map[8][12].endpoint = false;
		map[8][13].picetype = north;
		map[8][13].playerison = false;
		map[8][13].endpoint = false;
		map[8][14].picetype = north;
		map[8][14].playerison = false;
		map[8][14].endpoint = false;
		map[8][15].picetype = north;
		map[8][15].playerison = false;
		map[8][15].endpoint = false;
		map[8][16].picetype = north;
		map[8][16].playerison = false;
		map[8][16].endpoint = false;
		map[8][17].picetype = north;
		map[8][17].playerison = false;
		map[8][17].endpoint = false;
		map[8][18].picetype = north;
		map[8][18].playerison = false;
		map[8][18].endpoint = false;
		map[8][19].picetype = north;
		map[8][19].playerison = false;
		map[8][19].endpoint = false;
		map[8][20].picetype = north;
		map[8][20].playerison = false;
		map[8][20].endpoint = false;
		map[8][21].picetype = north;
		map[8][21].playerison = false;
		map[8][21].endpoint = false;
		map[8][22].picetype = north;
		map[8][22].playerison = false;
		map[8][22].endpoint = false;
		map[8][23].picetype = north;
		map[8][23].playerison = false;
		map[8][23].endpoint = false;
		map[8][24].picetype = north;
		map[8][24].playerison = false;
		map[8][24].endpoint = false;
		map[8][25].picetype = north;
		map[8][25].playerison = false;
		map[8][25].endpoint = false;
		map[8][26].picetype = north;
		map[8][26].playerison = false;
		map[8][26].endpoint = false;
		map[8][27].picetype = north;
		map[8][27].playerison = false;
		map[8][27].endpoint = false;
		map[8][28].picetype = north;
		map[8][28].playerison = false;
		map[8][28].endpoint = false;
		map[8][29].picetype = southeast;
		map[8][29].playerison = false;
		map[8][29].endpoint = false;
		map[9][0].picetype = northeast;
		map[9][0].playerison = false;
		map[9][0].endpoint = false;
		map[9][1].picetype = north;
		map[9][1].playerison = false;
		map[9][1].endpoint = false;
		map[9][2].picetype = north;
		map[9][2].playerison = false;
		map[9][2].endpoint = false;
		map[9][3].picetype = north;
		map[9][3].playerison = false;
		map[9][3].endpoint = false;
		map[9][4].picetype = north;
		map[9][4].playerison = false;
		map[9][4].endpoint = false;
		map[9][5].picetype = north;
		map[9][5].playerison = false;
		map[9][5].endpoint = false;
		map[9][6].picetype = north;
		map[9][6].playerison = false;
		map[9][6].endpoint = false;
		map[9][7].picetype = north;
		map[9][7].playerison = false;
		map[9][7].endpoint = false;
		map[9][8].picetype = north;
		map[9][8].playerison = false;
		map[9][8].endpoint = false;
		map[9][9].picetype = north;
		map[9][9].playerison = false;
		map[9][9].endpoint = false;
		map[9][10].picetype = north;
		map[9][10].playerison = false;
		map[9][10].endpoint = false;
		map[9][11].picetype = north;
		map[9][11].playerison = false;
		map[9][11].endpoint = false;
		map[9][12].picetype = north;
		map[9][12].playerison = false;
		map[9][12].endpoint = false;
		map[9][13].picetype = north;
		map[9][13].playerison = false;
		map[9][13].endpoint = false;
		map[9][14].picetype = north;
		map[9][14].playerison = false;
		map[9][14].endpoint = false;
		map[9][15].picetype = north;
		map[9][15].playerison = false;
		map[9][15].endpoint = false;
		map[9][16].picetype = north;
		map[9][16].playerison = false;
		map[9][16].endpoint = false;
		map[9][17].picetype = north;
		map[9][17].playerison = false;
		map[9][17].endpoint = false;
		map[9][18].picetype = north;
		map[9][18].playerison = false;
		map[9][18].endpoint = false;
		map[9][19].picetype = north;
		map[9][19].playerison = false;
		map[9][19].endpoint = false;
		map[9][20].picetype = north;
		map[9][20].playerison = false;
		map[9][20].endpoint = false;
		map[9][21].picetype = north;
		map[9][21].playerison = false;
		map[9][21].endpoint = false;
		map[9][22].picetype = north;
		map[9][22].playerison = false;
		map[9][22].endpoint = false;
		map[9][23].picetype = north;
		map[9][23].playerison = false;
		map[9][23].endpoint = false;
		map[9][24].picetype = north;
		map[9][24].playerison = false;
		map[9][24].endpoint = false;
		map[9][25].picetype = north;
		map[9][25].playerison = false;
		map[9][25].endpoint = false;
		map[9][26].picetype = north;
		map[9][26].playerison = false;
		map[9][26].endpoint = false;
		map[9][27].picetype = north;
		map[9][27].playerison = false;
		map[9][27].endpoint = false;
		map[9][28].picetype = north;
		map[9][28].playerison = false;
		map[9][28].endpoint = false;
		map[9][29].picetype = southwest;
		map[9][29].playerison = false;
		map[9][29].endpoint = false;
		map[10][0].picetype = northwest;
		map[10][0].playerison = false;
		map[10][0].endpoint = false;
		map[10][1].picetype = north;
		map[10][1].playerison = false;
		map[10][1].endpoint = false;
		map[10][2].picetype = north;
		map[10][2].playerison = false;
		map[10][2].endpoint = false;
		map[10][3].picetype = north;
		map[10][3].playerison = false;
		map[10][3].endpoint = false;
		map[10][4].picetype = north;
		map[10][4].playerison = false;
		map[10][4].endpoint = false;
		map[10][5].picetype = north;
		map[10][5].playerison = false;
		map[10][5].endpoint = false;
		map[10][6].picetype = north;
		map[10][6].playerison = false;
		map[10][6].endpoint = false;
		map[10][7].picetype = north;
		map[10][7].playerison = false;
		map[10][7].endpoint = false;
		map[10][8].picetype = north;
		map[10][8].playerison = false;
		map[10][8].endpoint = false;
		map[10][9].picetype = north;
		map[10][9].playerison = false;
		map[10][9].endpoint = false;
		map[10][10].picetype = north;
		map[10][10].playerison = false;
		map[10][10].endpoint = false;
		map[10][11].picetype = north;
		map[10][11].playerison = false;
		map[10][11].endpoint = false;
		map[10][12].picetype = north;
		map[10][12].playerison = false;
		map[10][12].endpoint = false;
		map[10][13].picetype = north;
		map[10][13].playerison = false;
		map[10][13].endpoint = false;
		map[10][14].picetype = north;
		map[10][14].playerison = false;
		map[10][14].endpoint = false;
		map[10][15].picetype = north;
		map[10][15].playerison = false;
		map[10][15].endpoint = false;
		map[10][16].picetype = north;
		map[10][16].playerison = false;
		map[10][16].endpoint = false;
		map[10][17].picetype = north;
		map[10][17].playerison = false;
		map[10][17].endpoint = false;
		map[10][18].picetype = north;
		map[10][18].playerison = false;
		map[10][18].endpoint = false;
		map[10][19].picetype = north;
		map[10][19].playerison = false;
		map[10][19].endpoint = false;
		map[10][20].picetype = north;
		map[10][20].playerison = false;
		map[10][20].endpoint = false;
		map[10][21].picetype = north;
		map[10][21].playerison = false;
		map[10][21].endpoint = false;
		map[10][22].picetype = north;
		map[10][22].playerison = false;
		map[10][22].endpoint = false;
		map[10][23].picetype = north;
		map[10][23].playerison = false;
		map[10][23].endpoint = false;
		map[10][24].picetype = north;
		map[10][24].playerison = false;
		map[10][24].endpoint = false;
		map[10][25].picetype = north;
		map[10][25].playerison = false;
		map[10][25].endpoint = false;
		map[10][26].picetype = north;
		map[10][26].playerison = false;
		map[10][26].endpoint = false;
		map[10][27].picetype = north;
		map[10][27].playerison = false;
		map[10][27].endpoint = false;
		map[10][28].picetype = north;
		map[10][28].playerison = false;
		map[10][28].endpoint = false;
		map[10][29].picetype = southeast;
		map[10][29].playerison = false;
		map[10][29].endpoint = false;
		map[11][0].picetype = northeast;
		map[11][0].playerison = false;
		map[11][0].endpoint = false;
		map[11][1].picetype = north;
		map[11][1].playerison = false;
		map[11][1].endpoint = false;
		map[11][2].picetype = north;
		map[11][2].playerison = false;
		map[11][2].endpoint = false;
		map[11][3].picetype = north;
		map[11][3].playerison = false;
		map[11][3].endpoint = false;
		map[11][4].picetype = north;
		map[11][4].playerison = false;
		map[11][4].endpoint = false;
		map[11][5].picetype = north;
		map[11][5].playerison = false;
		map[11][5].endpoint = false;
		map[11][6].picetype = north;
		map[11][6].playerison = false;
		map[11][6].endpoint = false;
		map[11][7].picetype = north;
		map[11][7].playerison = false;
		map[11][7].endpoint = false;
		map[11][8].picetype = north;
		map[11][8].playerison = false;
		map[11][8].endpoint = false;
		map[11][9].picetype = north;
		map[11][9].playerison = false;
		map[11][9].endpoint = false;
		map[11][10].picetype = north;
		map[11][10].playerison = false;
		map[11][10].endpoint = false;
		map[11][11].picetype = north;
		map[11][11].playerison = false;
		map[11][11].endpoint = false;
		map[11][12].picetype = north;
		map[11][12].playerison = false;
		map[11][12].endpoint = false;
		map[11][13].picetype = north;
		map[11][13].playerison = false;
		map[11][13].endpoint = false;
		map[11][14].picetype = north;
		map[11][14].playerison = false;
		map[11][14].endpoint = false;
		map[11][15].picetype = north;
		map[11][15].playerison = false;
		map[11][15].endpoint = false;
		map[11][16].picetype = north;
		map[11][16].playerison = false;
		map[11][16].endpoint = false;
		map[11][17].picetype = north;
		map[11][17].playerison = false;
		map[11][17].endpoint = false;
		map[11][18].picetype = north;
		map[11][18].playerison = false;
		map[11][18].endpoint = false;
		map[11][19].picetype = north;
		map[11][19].playerison = false;
		map[11][19].endpoint = false;
		map[11][20].picetype = north;
		map[11][20].playerison = false;
		map[11][20].endpoint = false;
		map[11][21].picetype = north;
		map[11][21].playerison = false;
		map[11][21].endpoint = false;
		map[11][22].picetype = north;
		map[11][22].playerison = false;
		map[11][22].endpoint = false;
		map[11][23].picetype = north;
		map[11][23].playerison = false;
		map[11][23].endpoint = false;
		map[11][24].picetype = north;
		map[11][24].playerison = false;
		map[11][24].endpoint = false;
		map[11][25].picetype = north;
		map[11][25].playerison = false;
		map[11][25].endpoint = false;
		map[11][26].picetype = north;
		map[11][26].playerison = false;
		map[11][26].endpoint = false;
		map[11][27].picetype = north;
		map[11][27].playerison = false;
		map[11][27].endpoint = false;
		map[11][28].picetype = north;
		map[11][28].playerison = false;
		map[11][28].endpoint = false;
		map[11][29].picetype = southwest;
		map[11][29].playerison = false;
		map[11][29].endpoint = false;
		map[12][0].picetype = northwest;
		map[12][0].playerison = false;
		map[12][0].endpoint = false;
		map[12][1].picetype = north;
		map[12][1].playerison = false;
		map[12][1].endpoint = false;
		map[12][2].picetype = north;
		map[12][2].playerison = false;
		map[12][2].endpoint = false;
		map[12][3].picetype = north;
		map[12][3].playerison = false;
		map[12][3].endpoint = false;
		map[12][4].picetype = north;
		map[12][4].playerison = false;
		map[12][4].endpoint = false;
		map[12][5].picetype = north;
		map[12][5].playerison = false;
		map[12][5].endpoint = false;
		map[12][6].picetype = north;
		map[12][6].playerison = false;
		map[12][6].endpoint = false;
		map[12][7].picetype = north;
		map[12][7].playerison = false;
		map[12][7].endpoint = false;
		map[12][8].picetype = north;
		map[12][8].playerison = false;
		map[12][8].endpoint = false;
		map[12][9].picetype = north;
		map[12][9].playerison = false;
		map[12][9].endpoint = false;
		map[12][10].picetype = north;
		map[12][10].playerison = false;
		map[12][10].endpoint = false;
		map[12][11].picetype = north;
		map[12][11].playerison = false;
		map[12][11].endpoint = false;
		map[12][12].picetype = north;
		map[12][12].playerison = false;
		map[12][12].endpoint = false;
		map[12][13].picetype = north;
		map[12][13].playerison = false;
		map[12][13].endpoint = false;
		map[12][14].picetype = north;
		map[12][14].playerison = false;
		map[12][14].endpoint = false;
		map[12][15].picetype = north;
		map[12][15].playerison = false;
		map[12][15].endpoint = false;
		map[12][16].picetype = north;
		map[12][16].playerison = false;
		map[12][16].endpoint = false;
		map[12][17].picetype = north;
		map[12][17].playerison = false;
		map[12][17].endpoint = false;
		map[12][18].picetype = north;
		map[12][18].playerison = false;
		map[12][18].endpoint = false;
		map[12][19].picetype = north;
		map[12][19].playerison = false;
		map[12][19].endpoint = false;
		map[12][20].picetype = north;
		map[12][20].playerison = false;
		map[12][20].endpoint = false;
		map[12][21].picetype = north;
		map[12][21].playerison = false;
		map[12][21].endpoint = false;
		map[12][22].picetype = north;
		map[12][22].playerison = false;
		map[12][22].endpoint = false;
		map[12][23].picetype = north;
		map[12][23].playerison = false;
		map[12][23].endpoint = false;
		map[12][24].picetype = north;
		map[12][24].playerison = false;
		map[12][24].endpoint = false;
		map[12][25].picetype = north;
		map[12][25].playerison = false;
		map[12][25].endpoint = false;
		map[12][26].picetype = north;
		map[12][26].playerison = false;
		map[12][26].endpoint = false;
		map[12][27].picetype = north;
		map[12][27].playerison = false;
		map[12][27].endpoint = false;
		map[12][28].picetype = north;
		map[12][28].playerison = false;
		map[12][28].endpoint = false;
		map[12][29].picetype = southeast;
		map[12][29].playerison = false;
		map[12][29].endpoint = false;
		map[13][0].picetype = northeast;
		map[13][0].playerison = false;
		map[13][0].endpoint = false;
		map[13][1].picetype = north;
		map[13][1].playerison = false;
		map[13][1].endpoint = false;
		map[13][2].picetype = north;
		map[13][2].playerison = false;
		map[13][2].endpoint = false;
		map[13][3].picetype = north;
		map[13][3].playerison = false;
		map[13][3].endpoint = false;
		map[13][4].picetype = north;
		map[13][4].playerison = false;
		map[13][4].endpoint = false;
		map[13][5].picetype = north;
		map[13][5].playerison = false;
		map[13][5].endpoint = false;
		map[13][6].picetype = north;
		map[13][6].playerison = false;
		map[13][6].endpoint = false;
		map[13][7].picetype = north;
		map[13][7].playerison = false;
		map[13][7].endpoint = false;
		map[13][8].picetype = north;
		map[13][8].playerison = false;
		map[13][8].endpoint = false;
		map[13][9].picetype = north;
		map[13][9].playerison = false;
		map[13][9].endpoint = false;
		map[13][10].picetype = north;
		map[13][10].playerison = false;
		map[13][10].endpoint = false;
		map[13][11].picetype = north;
		map[13][11].playerison = false;
		map[13][11].endpoint = false;
		map[13][12].picetype = north;
		map[13][12].playerison = false;
		map[13][12].endpoint = false;
		map[13][13].picetype = north;
		map[13][13].playerison = false;
		map[13][13].endpoint = false;
		map[13][14].picetype = north;
		map[13][14].playerison = false;
		map[13][14].endpoint = false;
		map[13][15].picetype = north;
		map[13][15].playerison = false;
		map[13][15].endpoint = false;
		map[13][16].picetype = north;
		map[13][16].playerison = false;
		map[13][16].endpoint = false;
		map[13][17].picetype = north;
		map[13][17].playerison = false;
		map[13][17].endpoint = false;
		map[13][18].picetype = north;
		map[13][18].playerison = false;
		map[13][18].endpoint = false;
		map[13][19].picetype = north;
		map[13][19].playerison = false;
		map[13][19].endpoint = false;
		map[13][20].picetype = north;
		map[13][20].playerison = false;
		map[13][20].endpoint = false;
		map[13][21].picetype = north;
		map[13][21].playerison = false;
		map[13][21].endpoint = false;
		map[13][22].picetype = north;
		map[13][22].playerison = false;
		map[13][22].endpoint = false;
		map[13][23].picetype = north;
		map[13][23].playerison = false;
		map[13][23].endpoint = false;
		map[13][24].picetype = north;
		map[13][24].playerison = false;
		map[13][24].endpoint = false;
		map[13][25].picetype = north;
		map[13][25].playerison = false;
		map[13][25].endpoint = false;
		map[13][26].picetype = north;
		map[13][26].playerison = false;
		map[13][26].endpoint = false;
		map[13][27].picetype = north;
		map[13][27].playerison = false;
		map[13][27].endpoint = false;
		map[13][28].picetype = north;
		map[13][28].playerison = false;
		map[13][28].endpoint = false;
		map[13][29].picetype = southwest;
		map[13][29].playerison = false;
		map[13][29].endpoint = false;
		map[14][0].picetype = northwest;
		map[14][0].playerison = false;
		map[14][0].endpoint = false;
		map[14][1].picetype = north;
		map[14][1].playerison = false;
		map[14][1].endpoint = false;
		map[14][2].picetype = north;
		map[14][2].playerison = false;
		map[14][2].endpoint = false;
		map[14][3].picetype = north;
		map[14][3].playerison = false;
		map[14][3].endpoint = false;
		map[14][4].picetype = north;
		map[14][4].playerison = false;
		map[14][4].endpoint = false;
		map[14][5].picetype = north;
		map[14][5].playerison = false;
		map[14][5].endpoint = false;
		map[14][6].picetype = north;
		map[14][6].playerison = false;
		map[14][6].endpoint = false;
		map[14][7].picetype = north;
		map[14][7].playerison = false;
		map[14][7].endpoint = false;
		map[14][8].picetype = north;
		map[14][8].playerison = false;
		map[14][8].endpoint = false;
		map[14][9].picetype = north;
		map[14][9].playerison = false;
		map[14][9].endpoint = false;
		map[14][10].picetype = north;
		map[14][10].playerison = false;
		map[14][10].endpoint = false;
		map[14][11].picetype = north;
		map[14][11].playerison = false;
		map[14][11].endpoint = false;
		map[14][12].picetype = north;
		map[14][12].playerison = false;
		map[14][12].endpoint = false;
		map[14][13].picetype = north;
		map[14][13].playerison = false;
		map[14][13].endpoint = false;
		map[14][14].picetype = north;
		map[14][14].playerison = false;
		map[14][14].endpoint = false;
		map[14][15].picetype = north;
		map[14][15].playerison = false;
		map[14][15].endpoint = false;
		map[14][16].picetype = north;
		map[14][16].playerison = false;
		map[14][16].endpoint = false;
		map[14][17].picetype = north;
		map[14][17].playerison = false;
		map[14][17].endpoint = false;
		map[14][18].picetype = north;
		map[14][18].playerison = false;
		map[14][18].endpoint = false;
		map[14][19].picetype = north;
		map[14][19].playerison = false;
		map[14][19].endpoint = false;
		map[14][20].picetype = north;
		map[14][20].playerison = false;
		map[14][20].endpoint = false;
		map[14][21].picetype = north;
		map[14][21].playerison = false;
		map[14][21].endpoint = false;
		map[14][22].picetype = north;
		map[14][22].playerison = false;
		map[14][22].endpoint = false;
		map[14][23].picetype = north;
		map[14][23].playerison = false;
		map[14][23].endpoint = false;
		map[14][24].picetype = north;
		map[14][24].playerison = false;
		map[14][24].endpoint = false;
		map[14][25].picetype = north;
		map[14][25].playerison = false;
		map[14][25].endpoint = false;
		map[14][26].picetype = north;
		map[14][26].playerison = false;
		map[14][26].endpoint = false;
		map[14][27].picetype = north;
		map[14][27].playerison = false;
		map[14][27].endpoint = false;
		map[14][28].picetype = north;
		map[14][28].playerison = false;
		map[14][28].endpoint = false;
		map[14][29].picetype = southeast;
		map[14][29].playerison = false;
		map[14][29].endpoint = false;
		map[15][0].picetype = northeast;
		map[15][0].playerison = false;
		map[15][0].endpoint = false;
		map[15][1].picetype = north;
		map[15][1].playerison = false;
		map[15][1].endpoint = false;
		map[15][2].picetype = north;
		map[15][2].playerison = false;
		map[15][2].endpoint = false;
		map[15][3].picetype = north;
		map[15][3].playerison = false;
		map[15][3].endpoint = false;
		map[15][4].picetype = north;
		map[15][4].playerison = false;
		map[15][4].endpoint = false;
		map[15][5].picetype = north;
		map[15][5].playerison = false;
		map[15][5].endpoint = false;
		map[15][6].picetype = north;
		map[15][6].playerison = false;
		map[15][6].endpoint = false;
		map[15][7].picetype = north;
		map[15][7].playerison = false;
		map[15][7].endpoint = false;
		map[15][8].picetype = north;
		map[15][8].playerison = false;
		map[15][8].endpoint = false;
		map[15][9].picetype = north;
		map[15][9].playerison = false;
		map[15][9].endpoint = false;
		map[15][10].picetype = north;
		map[15][10].playerison = false;
		map[15][10].endpoint = false;
		map[15][11].picetype = north;
		map[15][11].playerison = false;
		map[15][11].endpoint = false;
		map[15][12].picetype = north;
		map[15][12].playerison = false;
		map[15][12].endpoint = false;
		map[15][13].picetype = north;
		map[15][13].playerison = false;
		map[15][13].endpoint = false;
		map[15][14].picetype = north;
		map[15][14].playerison = false;
		map[15][14].endpoint = false;
		map[15][15].picetype = north;
		map[15][15].playerison = false;
		map[15][15].endpoint = false;
		map[15][16].picetype = north;
		map[15][16].playerison = false;
		map[15][16].endpoint = false;
		map[15][17].picetype = north;
		map[15][17].playerison = false;
		map[15][17].endpoint = false;
		map[15][18].picetype = north;
		map[15][18].playerison = false;
		map[15][18].endpoint = false;
		map[15][19].picetype = north;
		map[15][19].playerison = false;
		map[15][19].endpoint = false;
		map[15][20].picetype = north;
		map[15][20].playerison = false;
		map[15][20].endpoint = false;
		map[15][21].picetype = north;
		map[15][21].playerison = false;
		map[15][21].endpoint = false;
		map[15][22].picetype = north;
		map[15][22].playerison = false;
		map[15][22].endpoint = false;
		map[15][23].picetype = north;
		map[15][23].playerison = false;
		map[15][23].endpoint = false;
		map[15][24].picetype = north;
		map[15][24].playerison = false;
		map[15][24].endpoint = false;
		map[15][25].picetype = north;
		map[15][25].playerison = false;
		map[15][25].endpoint = false;
		map[15][26].picetype = north;
		map[15][26].playerison = false;
		map[15][26].endpoint = false;
		map[15][27].picetype = north;
		map[15][27].playerison = false;
		map[15][27].endpoint = false;
		map[15][28].picetype = north;
		map[15][28].playerison = false;
		map[15][28].endpoint = false;
		map[15][29].picetype = southwest;
		map[15][29].playerison = false;
		map[15][29].endpoint = false;
		map[16][0].picetype = northwest;
		map[16][0].playerison = false;
		map[16][0].endpoint = false;
		map[16][1].picetype = north;
		map[16][1].playerison = false;
		map[16][1].endpoint = false;
		map[16][2].picetype = north;
		map[16][2].playerison = false;
		map[16][2].endpoint = false;
		map[16][3].picetype = north;
		map[16][3].playerison = false;
		map[16][3].endpoint = false;
		map[16][4].picetype = north;
		map[16][4].playerison = false;
		map[16][4].endpoint = false;
		map[16][5].picetype = north;
		map[16][5].playerison = false;
		map[16][5].endpoint = false;
		map[16][6].picetype = north;
		map[16][6].playerison = false;
		map[16][6].endpoint = false;
		map[16][7].picetype = north;
		map[16][7].playerison = false;
		map[16][7].endpoint = false;
		map[16][8].picetype = north;
		map[16][8].playerison = false;
		map[16][8].endpoint = false;
		map[16][9].picetype = north;
		map[16][9].playerison = false;
		map[16][9].endpoint = false;
		map[16][10].picetype = north;
		map[16][10].playerison = false;
		map[16][10].endpoint = false;
		map[16][11].picetype = north;
		map[16][11].playerison = false;
		map[16][11].endpoint = false;
		map[16][12].picetype = north;
		map[16][12].playerison = false;
		map[16][12].endpoint = false;
		map[16][13].picetype = north;
		map[16][13].playerison = false;
		map[16][13].endpoint = false;
		map[16][14].picetype = north;
		map[16][14].playerison = false;
		map[16][14].endpoint = false;
		map[16][15].picetype = north;
		map[16][15].playerison = false;
		map[16][15].endpoint = false;
		map[16][16].picetype = north;
		map[16][16].playerison = false;
		map[16][16].endpoint = false;
		map[16][17].picetype = north;
		map[16][17].playerison = false;
		map[16][17].endpoint = false;
		map[16][18].picetype = north;
		map[16][18].playerison = false;
		map[16][18].endpoint = false;
		map[16][19].picetype = north;
		map[16][19].playerison = false;
		map[16][19].endpoint = false;
		map[16][20].picetype = north;
		map[16][20].playerison = false;
		map[16][20].endpoint = false;
		map[16][21].picetype = north;
		map[16][21].playerison = false;
		map[16][21].endpoint = false;
		map[16][22].picetype = north;
		map[16][22].playerison = false;
		map[16][22].endpoint = false;
		map[16][23].picetype = north;
		map[16][23].playerison = false;
		map[16][23].endpoint = false;
		map[16][24].picetype = north;
		map[16][24].playerison = false;
		map[16][24].endpoint = false;
		map[16][25].picetype = north;
		map[16][25].playerison = false;
		map[16][25].endpoint = false;
		map[16][26].picetype = north;
		map[16][26].playerison = false;
		map[16][26].endpoint = false;
		map[16][27].picetype = north;
		map[16][27].playerison = false;
		map[16][27].endpoint = false;
		map[16][28].picetype = north;
		map[16][28].playerison = false;
		map[16][28].endpoint = false;
		map[16][29].picetype = southeast;
		map[16][29].playerison = false;
		map[16][29].endpoint = false;
		map[17][0].picetype = northeast;
		map[17][0].playerison = false;
		map[17][0].endpoint = false;
		map[17][1].picetype = north;
		map[17][1].playerison = false;
		map[17][1].endpoint = false;
		map[17][2].picetype = north;
		map[17][2].playerison = false;
		map[17][2].endpoint = false;
		map[17][3].picetype = north;
		map[17][3].playerison = false;
		map[17][3].endpoint = false;
		map[17][4].picetype = north;
		map[17][4].playerison = false;
		map[17][4].endpoint = false;
		map[17][5].picetype = north;
		map[17][5].playerison = false;
		map[17][5].endpoint = false;
		map[17][6].picetype = north;
		map[17][6].playerison = false;
		map[17][6].endpoint = false;
		map[17][7].picetype = north;
		map[17][7].playerison = false;
		map[17][7].endpoint = false;
		map[17][8].picetype = north;
		map[17][8].playerison = false;
		map[17][8].endpoint = false;
		map[17][9].picetype = north;
		map[17][9].playerison = false;
		map[17][9].endpoint = false;
		map[17][10].picetype = north;
		map[17][10].playerison = false;
		map[17][10].endpoint = false;
		map[17][11].picetype = north;
		map[17][11].playerison = false;
		map[17][11].endpoint = false;
		map[17][12].picetype = north;
		map[17][12].playerison = false;
		map[17][12].endpoint = false;
		map[17][13].picetype = north;
		map[17][13].playerison = false;
		map[17][13].endpoint = false;
		map[17][14].picetype = north;
		map[17][14].playerison = false;
		map[17][14].endpoint = false;
		map[17][15].picetype = north;
		map[17][15].playerison = false;
		map[17][15].endpoint = false;
		map[17][16].picetype = north;
		map[17][16].playerison = false;
		map[17][16].endpoint = false;
		map[17][17].picetype = north;
		map[17][17].playerison = false;
		map[17][17].endpoint = false;
		map[17][18].picetype = north;
		map[17][18].playerison = false;
		map[17][18].endpoint = false;
		map[17][19].picetype = north;
		map[17][19].playerison = false;
		map[17][19].endpoint = false;
		map[17][20].picetype = north;
		map[17][20].playerison = false;
		map[17][20].endpoint = false;
		map[17][21].picetype = north;
		map[17][21].playerison = false;
		map[17][21].endpoint = false;
		map[17][22].picetype = north;
		map[17][22].playerison = false;
		map[17][22].endpoint = false;
		map[17][23].picetype = north;
		map[17][23].playerison = false;
		map[17][23].endpoint = false;
		map[17][24].picetype = north;
		map[17][24].playerison = false;
		map[17][24].endpoint = false;
		map[17][25].picetype = north;
		map[17][25].playerison = false;
		map[17][25].endpoint = false;
		map[17][26].picetype = north;
		map[17][26].playerison = false;
		map[17][26].endpoint = false;
		map[17][27].picetype = north;
		map[17][27].playerison = false;
		map[17][27].endpoint = false;
		map[17][28].picetype = north;
		map[17][28].playerison = false;
		map[17][28].endpoint = false;
		map[17][29].picetype = southwest;
		map[17][29].playerison = false;
		map[17][29].endpoint = false;
		map[18][0].picetype = northwest;
		map[18][0].playerison = false;
		map[18][0].endpoint = false;
		map[18][1].picetype = north;
		map[18][1].playerison = false;
		map[18][1].endpoint = false;
		map[18][2].picetype = north;
		map[18][2].playerison = false;
		map[18][2].endpoint = false;
		map[18][3].picetype = north;
		map[18][3].playerison = false;
		map[18][3].endpoint = false;
		map[18][4].picetype = north;
		map[18][4].playerison = false;
		map[18][4].endpoint = false;
		map[18][5].picetype = north;
		map[18][5].playerison = false;
		map[18][5].endpoint = false;
		map[18][6].picetype = north;
		map[18][6].playerison = false;
		map[18][6].endpoint = false;
		map[18][7].picetype = north;
		map[18][7].playerison = false;
		map[18][7].endpoint = false;
		map[18][8].picetype = north;
		map[18][8].playerison = false;
		map[18][8].endpoint = false;
		map[18][9].picetype = north;
		map[18][9].playerison = false;
		map[18][9].endpoint = false;
		map[18][10].picetype = north;
		map[18][10].playerison = false;
		map[18][10].endpoint = false;
		map[18][11].picetype = north;
		map[18][11].playerison = false;
		map[18][11].endpoint = false;
		map[18][12].picetype = north;
		map[18][12].playerison = false;
		map[18][12].endpoint = false;
		map[18][13].picetype = north;
		map[18][13].playerison = false;
		map[18][13].endpoint = false;
		map[18][14].picetype = north;
		map[18][14].playerison = false;
		map[18][14].endpoint = false;
		map[18][15].picetype = north;
		map[18][15].playerison = false;
		map[18][15].endpoint = false;
		map[18][16].picetype = north;
		map[18][16].playerison = false;
		map[18][16].endpoint = false;
		map[18][17].picetype = north;
		map[18][17].playerison = false;
		map[18][17].endpoint = false;
		map[18][18].picetype = north;
		map[18][18].playerison = false;
		map[18][18].endpoint = false;
		map[18][19].picetype = north;
		map[18][19].playerison = false;
		map[18][19].endpoint = false;
		map[18][20].picetype = north;
		map[18][20].playerison = false;
		map[18][20].endpoint = false;
		map[18][21].picetype = north;
		map[18][21].playerison = false;
		map[18][21].endpoint = false;
		map[18][22].picetype = north;
		map[18][22].playerison = false;
		map[18][22].endpoint = false;
		map[18][23].picetype = north;
		map[18][23].playerison = false;
		map[18][23].endpoint = false;
		map[18][24].picetype = north;
		map[18][24].playerison = false;
		map[18][24].endpoint = false;
		map[18][25].picetype = north;
		map[18][25].playerison = false;
		map[18][25].endpoint = false;
		map[18][26].picetype = north;
		map[18][26].playerison = false;
		map[18][26].endpoint = false;
		map[18][27].picetype = north;
		map[18][27].playerison = false;
		map[18][27].endpoint = false;
		map[18][28].picetype = north;
		map[18][28].playerison = false;
		map[18][28].endpoint = false;
		map[18][29].picetype = southeast;
		map[18][29].playerison = false;
		map[18][29].endpoint = false;
		map[19][0].picetype = northeast;
		map[19][0].playerison = false;
		map[19][0].endpoint = false;
		map[19][1].picetype = north;
		map[19][1].playerison = false;
		map[19][1].endpoint = false;
		map[19][2].picetype = north;
		map[19][2].playerison = false;
		map[19][2].endpoint = false;
		map[19][3].picetype = north;
		map[19][3].playerison = false;
		map[19][3].endpoint = false;
		map[19][4].picetype = north;
		map[19][4].playerison = false;
		map[19][4].endpoint = false;
		map[19][5].picetype = north;
		map[19][5].playerison = false;
		map[19][5].endpoint = false;
		map[19][6].picetype = north;
		map[19][6].playerison = false;
		map[19][6].endpoint = false;
		map[19][7].picetype = north;
		map[19][7].playerison = false;
		map[19][7].endpoint = false;
		map[19][8].picetype = north;
		map[19][8].playerison = false;
		map[19][8].endpoint = false;
		map[19][9].picetype = north;
		map[19][9].playerison = false;
		map[19][9].endpoint = false;
		map[19][10].picetype = north;
		map[19][10].playerison = false;
		map[19][10].endpoint = false;
		map[19][11].picetype = north;
		map[19][11].playerison = false;
		map[19][11].endpoint = false;
		map[19][12].picetype = north;
		map[19][12].playerison = false;
		map[19][12].endpoint = false;
		map[19][13].picetype = north;
		map[19][13].playerison = false;
		map[19][13].endpoint = false;
		map[19][14].picetype = north;
		map[19][14].playerison = false;
		map[19][14].endpoint = false;
		map[19][15].picetype = north;
		map[19][15].playerison = false;
		map[19][15].endpoint = false;
		map[19][16].picetype = north;
		map[19][16].playerison = false;
		map[19][16].endpoint = false;
		map[19][17].picetype = north;
		map[19][17].playerison = false;
		map[19][17].endpoint = false;
		map[19][18].picetype = north;
		map[19][18].playerison = false;
		map[19][18].endpoint = false;
		map[19][19].picetype = north;
		map[19][19].playerison = false;
		map[19][19].endpoint = false;
		map[19][20].picetype = north;
		map[19][20].playerison = false;
		map[19][20].endpoint = false;
		map[19][21].picetype = north;
		map[19][21].playerison = false;
		map[19][21].endpoint = false;
		map[19][22].picetype = north;
		map[19][22].playerison = false;
		map[19][22].endpoint = false;
		map[19][23].picetype = north;
		map[19][23].playerison = false;
		map[19][23].endpoint = false;
		map[19][24].picetype = north;
		map[19][24].playerison = false;
		map[19][24].endpoint = false;
		map[19][25].picetype = north;
		map[19][25].playerison = false;
		map[19][25].endpoint = false;
		map[19][26].picetype = north;
		map[19][26].playerison = false;
		map[19][26].endpoint = false;
		map[19][27].picetype = north;
		map[19][27].playerison = false;
		map[19][27].endpoint = false;
		map[19][28].picetype = north;
		map[19][28].playerison = false;
		map[19][28].endpoint = false;
		map[19][29].picetype = southwest;
		map[19][29].playerison = false;
		map[19][29].endpoint = false;
		map[20][0].picetype = northwest;
		map[20][0].playerison = false;
		map[20][0].endpoint = false;
		map[20][1].picetype = north;
		map[20][1].playerison = false;
		map[20][1].endpoint = false;
		map[20][2].picetype = north;
		map[20][2].playerison = false;
		map[20][2].endpoint = false;
		map[20][3].picetype = north;
		map[20][3].playerison = false;
		map[20][3].endpoint = false;
		map[20][4].picetype = north;
		map[20][4].playerison = false;
		map[20][4].endpoint = false;
		map[20][5].picetype = north;
		map[20][5].playerison = false;
		map[20][5].endpoint = false;
		map[20][6].picetype = north;
		map[20][6].playerison = false;
		map[20][6].endpoint = false;
		map[20][7].picetype = north;
		map[20][7].playerison = false;
		map[20][7].endpoint = false;
		map[20][8].picetype = north;
		map[20][8].playerison = false;
		map[20][8].endpoint = false;
		map[20][9].picetype = north;
		map[20][9].playerison = false;
		map[20][9].endpoint = false;
		map[20][10].picetype = north;
		map[20][10].playerison = false;
		map[20][10].endpoint = false;
		map[20][11].picetype = north;
		map[20][11].playerison = false;
		map[20][11].endpoint = false;
		map[20][12].picetype = north;
		map[20][12].playerison = false;
		map[20][12].endpoint = false;
		map[20][13].picetype = north;
		map[20][13].playerison = false;
		map[20][13].endpoint = false;
		map[20][14].picetype = north;
		map[20][14].playerison = false;
		map[20][14].endpoint = false;
		map[20][15].picetype = north;
		map[20][15].playerison = false;
		map[20][15].endpoint = false;
		map[20][16].picetype = north;
		map[20][16].playerison = false;
		map[20][16].endpoint = false;
		map[20][17].picetype = north;
		map[20][17].playerison = false;
		map[20][17].endpoint = false;
		map[20][18].picetype = north;
		map[20][18].playerison = false;
		map[20][18].endpoint = false;
		map[20][19].picetype = north;
		map[20][19].playerison = false;
		map[20][19].endpoint = false;
		map[20][20].picetype = north;
		map[20][20].playerison = false;
		map[20][20].endpoint = false;
		map[20][21].picetype = north;
		map[20][21].playerison = false;
		map[20][21].endpoint = false;
		map[20][22].picetype = north;
		map[20][22].playerison = false;
		map[20][22].endpoint = false;
		map[20][23].picetype = north;
		map[20][23].playerison = false;
		map[20][23].endpoint = false;
		map[20][24].picetype = north;
		map[20][24].playerison = false;
		map[20][24].endpoint = false;
		map[20][25].picetype = north;
		map[20][25].playerison = false;
		map[20][25].endpoint = false;
		map[20][26].picetype = north;
		map[20][26].playerison = false;
		map[20][26].endpoint = false;
		map[20][27].picetype = north;
		map[20][27].playerison = false;
		map[20][27].endpoint = false;
		map[20][28].picetype = north;
		map[20][28].playerison = false;
		map[20][28].endpoint = false;
		map[20][29].picetype = southeast;
		map[20][29].playerison = false;
		map[20][29].endpoint = false;
		map[21][0].picetype = northeast;
		map[21][0].playerison = false;
		map[21][0].endpoint = false;
		map[21][1].picetype = north;
		map[21][1].playerison = false;
		map[21][1].endpoint = false;
		map[21][2].picetype = north;
		map[21][2].playerison = false;
		map[21][2].endpoint = false;
		map[21][3].picetype = north;
		map[21][3].playerison = false;
		map[21][3].endpoint = false;
		map[21][4].picetype = north;
		map[21][4].playerison = false;
		map[21][4].endpoint = false;
		map[21][5].picetype = north;
		map[21][5].playerison = false;
		map[21][5].endpoint = false;
		map[21][6].picetype = north;
		map[21][6].playerison = false;
		map[21][6].endpoint = false;
		map[21][7].picetype = north;
		map[21][7].playerison = false;
		map[21][7].endpoint = false;
		map[21][8].picetype = north;
		map[21][8].playerison = false;
		map[21][8].endpoint = false;
		map[21][9].picetype = north;
		map[21][9].playerison = false;
		map[21][9].endpoint = false;
		map[21][10].picetype = north;
		map[21][10].playerison = false;
		map[21][10].endpoint = false;
		map[21][11].picetype = north;
		map[21][11].playerison = false;
		map[21][11].endpoint = false;
		map[21][12].picetype = north;
		map[21][12].playerison = false;
		map[21][12].endpoint = false;
		map[21][13].picetype = north;
		map[21][13].playerison = false;
		map[21][13].endpoint = false;
		map[21][14].picetype = north;
		map[21][14].playerison = false;
		map[21][14].endpoint = false;
		map[21][15].picetype = north;
		map[21][15].playerison = false;
		map[21][15].endpoint = false;
		map[21][16].picetype = north;
		map[21][16].playerison = false;
		map[21][16].endpoint = false;
		map[21][17].picetype = north;
		map[21][17].playerison = false;
		map[21][17].endpoint = false;
		map[21][18].picetype = north;
		map[21][18].playerison = false;
		map[21][18].endpoint = false;
		map[21][19].picetype = north;
		map[21][19].playerison = false;
		map[21][19].endpoint = false;
		map[21][20].picetype = north;
		map[21][20].playerison = false;
		map[21][20].endpoint = false;
		map[21][21].picetype = north;
		map[21][21].playerison = false;
		map[21][21].endpoint = false;
		map[21][22].picetype = north;
		map[21][22].playerison = false;
		map[21][22].endpoint = false;
		map[21][23].picetype = north;
		map[21][23].playerison = false;
		map[21][23].endpoint = false;
		map[21][24].picetype = north;
		map[21][24].playerison = false;
		map[21][24].endpoint = false;
		map[21][25].picetype = north;
		map[21][25].playerison = false;
		map[21][25].endpoint = false;
		map[21][26].picetype = north;
		map[21][26].playerison = false;
		map[21][26].endpoint = false;
		map[21][27].picetype = north;
		map[21][27].playerison = false;
		map[21][27].endpoint = false;
		map[21][28].picetype = north;
		map[21][28].playerison = false;
		map[21][28].endpoint = false;
		map[21][29].picetype = southwest;
		map[21][29].playerison = false;
		map[21][29].endpoint = false;
		map[22][0].picetype = northwest;
		map[22][0].playerison = false;
		map[22][0].endpoint = false;
		map[22][1].picetype = north;
		map[22][1].playerison = false;
		map[22][1].endpoint = false;
		map[22][2].picetype = north;
		map[22][2].playerison = false;
		map[22][2].endpoint = false;
		map[22][3].picetype = north;
		map[22][3].playerison = false;
		map[22][3].endpoint = false;
		map[22][4].picetype = north;
		map[22][4].playerison = false;
		map[22][4].endpoint = false;
		map[22][5].picetype = north;
		map[22][5].playerison = false;
		map[22][5].endpoint = false;
		map[22][6].picetype = north;
		map[22][6].playerison = false;
		map[22][6].endpoint = false;
		map[22][7].picetype = north;
		map[22][7].playerison = false;
		map[22][7].endpoint = false;
		map[22][8].picetype = north;
		map[22][8].playerison = false;
		map[22][8].endpoint = false;
		map[22][9].picetype = north;
		map[22][9].playerison = false;
		map[22][9].endpoint = false;
		map[22][10].picetype = north;
		map[22][10].playerison = false;
		map[22][10].endpoint = false;
		map[22][11].picetype = north;
		map[22][11].playerison = false;
		map[22][11].endpoint = false;
		map[22][12].picetype = north;
		map[22][12].playerison = false;
		map[22][12].endpoint = false;
		map[22][13].picetype = north;
		map[22][13].playerison = false;
		map[22][13].endpoint = false;
		map[22][14].picetype = north;
		map[22][14].playerison = false;
		map[22][14].endpoint = false;
		map[22][15].picetype = north;
		map[22][15].playerison = false;
		map[22][15].endpoint = false;
		map[22][16].picetype = north;
		map[22][16].playerison = false;
		map[22][16].endpoint = false;
		map[22][17].picetype = north;
		map[22][17].playerison = false;
		map[22][17].endpoint = false;
		map[22][18].picetype = north;
		map[22][18].playerison = false;
		map[22][18].endpoint = false;
		map[22][19].picetype = north;
		map[22][19].playerison = false;
		map[22][19].endpoint = false;
		map[22][20].picetype = north;
		map[22][20].playerison = false;
		map[22][20].endpoint = false;
		map[22][21].picetype = north;
		map[22][21].playerison = false;
		map[22][21].endpoint = false;
		map[22][22].picetype = north;
		map[22][22].playerison = false;
		map[22][22].endpoint = false;
		map[22][23].picetype = north;
		map[22][23].playerison = false;
		map[22][23].endpoint = false;
		map[22][24].picetype = north;
		map[22][24].playerison = false;
		map[22][24].endpoint = false;
		map[22][25].picetype = north;
		map[22][25].playerison = false;
		map[22][25].endpoint = false;
		map[22][26].picetype = north;
		map[22][26].playerison = false;
		map[22][26].endpoint = false;
		map[22][27].picetype = north;
		map[22][27].playerison = false;
		map[22][27].endpoint = false;
		map[22][28].picetype = north;
		map[22][28].playerison = false;
		map[22][28].endpoint = false;
		map[22][29].picetype = southeast;
		map[22][29].playerison = false;
		map[22][29].endpoint = false;
		map[23][0].picetype = northeast;
		map[23][0].playerison = false;
		map[23][0].endpoint = false;
		map[23][1].picetype = north;
		map[23][1].playerison = false;
		map[23][1].endpoint = false;
		map[23][2].picetype = north;
		map[23][2].playerison = false;
		map[23][2].endpoint = false;
		map[23][3].picetype = north;
		map[23][3].playerison = false;
		map[23][3].endpoint = false;
		map[23][4].picetype = north;
		map[23][4].playerison = false;
		map[23][4].endpoint = false;
		map[23][5].picetype = north;
		map[23][5].playerison = false;
		map[23][5].endpoint = false;
		map[23][6].picetype = north;
		map[23][6].playerison = false;
		map[23][6].endpoint = false;
		map[23][7].picetype = north;
		map[23][7].playerison = false;
		map[23][7].endpoint = false;
		map[23][8].picetype = north;
		map[23][8].playerison = false;
		map[23][8].endpoint = false;
		map[23][9].picetype = north;
		map[23][9].playerison = false;
		map[23][9].endpoint = false;
		map[23][10].picetype = north;
		map[23][10].playerison = false;
		map[23][10].endpoint = false;
		map[23][11].picetype = north;
		map[23][11].playerison = false;
		map[23][11].endpoint = false;
		map[23][12].picetype = north;
		map[23][12].playerison = false;
		map[23][12].endpoint = false;
		map[23][13].picetype = north;
		map[23][13].playerison = false;
		map[23][13].endpoint = false;
		map[23][14].picetype = north;
		map[23][14].playerison = false;
		map[23][14].endpoint = false;
		map[23][15].picetype = north;
		map[23][15].playerison = false;
		map[23][15].endpoint = false;
		map[23][16].picetype = north;
		map[23][16].playerison = false;
		map[23][16].endpoint = false;
		map[23][17].picetype = north;
		map[23][17].playerison = false;
		map[23][17].endpoint = false;
		map[23][18].picetype = north;
		map[23][18].playerison = false;
		map[23][18].endpoint = false;
		map[23][19].picetype = north;
		map[23][19].playerison = false;
		map[23][19].endpoint = false;
		map[23][20].picetype = north;
		map[23][20].playerison = false;
		map[23][20].endpoint = false;
		map[23][21].picetype = north;
		map[23][21].playerison = false;
		map[23][21].endpoint = false;
		map[23][22].picetype = north;
		map[23][22].playerison = false;
		map[23][22].endpoint = false;
		map[23][23].picetype = north;
		map[23][23].playerison = false;
		map[23][23].endpoint = false;
		map[23][24].picetype = north;
		map[23][24].playerison = false;
		map[23][24].endpoint = false;
		map[23][25].picetype = north;
		map[23][25].playerison = false;
		map[23][25].endpoint = false;
		map[23][26].picetype = north;
		map[23][26].playerison = false;
		map[23][26].endpoint = false;
		map[23][27].picetype = north;
		map[23][27].playerison = false;
		map[23][27].endpoint = false;
		map[23][28].picetype = north;
		map[23][28].playerison = false;
		map[23][28].endpoint = false;
		map[23][29].picetype = southwest;
		map[23][29].playerison = false;
		map[23][29].endpoint = false;
		map[24][0].picetype = northwest;
		map[24][0].playerison = false;
		map[24][0].endpoint = false;
		map[24][1].picetype = north;
		map[24][1].playerison = false;
		map[24][1].endpoint = false;
		map[24][2].picetype = north;
		map[24][2].playerison = false;
		map[24][2].endpoint = false;
		map[24][3].picetype = north;
		map[24][3].playerison = false;
		map[24][3].endpoint = false;
		map[24][4].picetype = north;
		map[24][4].playerison = false;
		map[24][4].endpoint = false;
		map[24][5].picetype = north;
		map[24][5].playerison = false;
		map[24][5].endpoint = false;
		map[24][6].picetype = north;
		map[24][6].playerison = false;
		map[24][6].endpoint = false;
		map[24][7].picetype = north;
		map[24][7].playerison = false;
		map[24][7].endpoint = false;
		map[24][8].picetype = north;
		map[24][8].playerison = false;
		map[24][8].endpoint = false;
		map[24][9].picetype = north;
		map[24][9].playerison = false;
		map[24][9].endpoint = false;
		map[24][10].picetype = north;
		map[24][10].playerison = false;
		map[24][10].endpoint = false;
		map[24][11].picetype = north;
		map[24][11].playerison = false;
		map[24][11].endpoint = false;
		map[24][12].picetype = north;
		map[24][12].playerison = false;
		map[24][12].endpoint = false;
		map[24][13].picetype = north;
		map[24][13].playerison = false;
		map[24][13].endpoint = false;
		map[24][14].picetype = north;
		map[24][14].playerison = false;
		map[24][14].endpoint = false;
		map[24][15].picetype = north;
		map[24][15].playerison = false;
		map[24][15].endpoint = false;
		map[24][16].picetype = north;
		map[24][16].playerison = false;
		map[24][16].endpoint = false;
		map[24][17].picetype = north;
		map[24][17].playerison = false;
		map[24][17].endpoint = false;
		map[24][18].picetype = north;
		map[24][18].playerison = false;
		map[24][18].endpoint = false;
		map[24][19].picetype = north;
		map[24][19].playerison = false;
		map[24][19].endpoint = false;
		map[24][20].picetype = north;
		map[24][20].playerison = false;
		map[24][20].endpoint = false;
		map[24][21].picetype = north;
		map[24][21].playerison = false;
		map[24][21].endpoint = false;
		map[24][22].picetype = north;
		map[24][22].playerison = false;
		map[24][22].endpoint = false;
		map[24][23].picetype = north;
		map[24][23].playerison = false;
		map[24][23].endpoint = false;
		map[24][24].picetype = north;
		map[24][24].playerison = false;
		map[24][24].endpoint = false;
		map[24][25].picetype = north;
		map[24][25].playerison = false;
		map[24][25].endpoint = false;
		map[24][26].picetype = north;
		map[24][26].playerison = false;
		map[24][26].endpoint = false;
		map[24][27].picetype = north;
		map[24][27].playerison = false;
		map[24][27].endpoint = false;
		map[24][28].picetype = north;
		map[24][28].playerison = false;
		map[24][28].endpoint = false;
		map[24][29].picetype = southeast;
		map[24][29].playerison = false;
		map[24][29].endpoint = false;
		map[25][0].picetype = northeast;
		map[25][0].playerison = false;
		map[25][0].endpoint = false;
		map[25][1].picetype = north;
		map[25][1].playerison = false;
		map[25][1].endpoint = false;
		map[25][2].picetype = north;
		map[25][2].playerison = false;
		map[25][2].endpoint = false;
		map[25][3].picetype = north;
		map[25][3].playerison = false;
		map[25][3].endpoint = false;
		map[25][4].picetype = north;
		map[25][4].playerison = false;
		map[25][4].endpoint = false;
		map[25][5].picetype = north;
		map[25][5].playerison = false;
		map[25][5].endpoint = false;
		map[25][6].picetype = north;
		map[25][6].playerison = false;
		map[25][6].endpoint = false;
		map[25][7].picetype = north;
		map[25][7].playerison = false;
		map[25][7].endpoint = false;
		map[25][8].picetype = north;
		map[25][8].playerison = false;
		map[25][8].endpoint = false;
		map[25][9].picetype = north;
		map[25][9].playerison = false;
		map[25][9].endpoint = false;
		map[25][10].picetype = north;
		map[25][10].playerison = false;
		map[25][10].endpoint = false;
		map[25][11].picetype = north;
		map[25][11].playerison = false;
		map[25][11].endpoint = false;
		map[25][12].picetype = north;
		map[25][12].playerison = false;
		map[25][12].endpoint = false;
		map[25][13].picetype = north;
		map[25][13].playerison = false;
		map[25][13].endpoint = false;
		map[25][14].picetype = north;
		map[25][14].playerison = false;
		map[25][14].endpoint = false;
		map[25][15].picetype = north;
		map[25][15].playerison = false;
		map[25][15].endpoint = false;
		map[25][16].picetype = north;
		map[25][16].playerison = false;
		map[25][16].endpoint = false;
		map[25][17].picetype = north;
		map[25][17].playerison = false;
		map[25][17].endpoint = false;
		map[25][18].picetype = north;
		map[25][18].playerison = false;
		map[25][18].endpoint = false;
		map[25][19].picetype = north;
		map[25][19].playerison = false;
		map[25][19].endpoint = false;
		map[25][20].picetype = north;
		map[25][20].playerison = false;
		map[25][20].endpoint = false;
		map[25][21].picetype = north;
		map[25][21].playerison = false;
		map[25][21].endpoint = false;
		map[25][22].picetype = north;
		map[25][22].playerison = false;
		map[25][22].endpoint = false;
		map[25][23].picetype = north;
		map[25][23].playerison = false;
		map[25][23].endpoint = false;
		map[25][24].picetype = north;
		map[25][24].playerison = false;
		map[25][24].endpoint = false;
		map[25][25].picetype = north;
		map[25][25].playerison = false;
		map[25][25].endpoint = false;
		map[25][26].picetype = north;
		map[25][26].playerison = false;
		map[25][26].endpoint = false;
		map[25][27].picetype = north;
		map[25][27].playerison = false;
		map[25][27].endpoint = false;
		map[25][28].picetype = north;
		map[25][28].playerison = false;
		map[25][28].endpoint = false;
		map[25][29].picetype = southwest;
		map[25][29].playerison = false;
		map[25][29].endpoint = false;
		map[26][0].picetype = northwest;
		map[26][0].playerison = false;
		map[26][0].endpoint = false;
		map[26][1].picetype = north;
		map[26][1].playerison = false;
		map[26][1].endpoint = false;
		map[26][2].picetype = north;
		map[26][2].playerison = false;
		map[26][2].endpoint = false;
		map[26][3].picetype = north;
		map[26][3].playerison = false;
		map[26][3].endpoint = false;
		map[26][4].picetype = north;
		map[26][4].playerison = false;
		map[26][4].endpoint = false;
		map[26][5].picetype = north;
		map[26][5].playerison = false;
		map[26][5].endpoint = false;
		map[26][6].picetype = north;
		map[26][6].playerison = false;
		map[26][6].endpoint = false;
		map[26][7].picetype = north;
		map[26][7].playerison = false;
		map[26][7].endpoint = false;
		map[26][8].picetype = north;
		map[26][8].playerison = false;
		map[26][8].endpoint = false;
		map[26][9].picetype = north;
		map[26][9].playerison = false;
		map[26][9].endpoint = false;
		map[26][10].picetype = north;
		map[26][10].playerison = false;
		map[26][10].endpoint = false;
		map[26][11].picetype = north;
		map[26][11].playerison = false;
		map[26][11].endpoint = false;
		map[26][12].picetype = north;
		map[26][12].playerison = false;
		map[26][12].endpoint = false;
		map[26][13].picetype = north;
		map[26][13].playerison = false;
		map[26][13].endpoint = false;
		map[26][14].picetype = north;
		map[26][14].playerison = false;
		map[26][14].endpoint = false;
		map[26][15].picetype = north;
		map[26][15].playerison = false;
		map[26][15].endpoint = false;
		map[26][16].picetype = north;
		map[26][16].playerison = false;
		map[26][16].endpoint = false;
		map[26][17].picetype = north;
		map[26][17].playerison = false;
		map[26][17].endpoint = false;
		map[26][18].picetype = north;
		map[26][18].playerison = false;
		map[26][18].endpoint = false;
		map[26][19].picetype = north;
		map[26][19].playerison = false;
		map[26][19].endpoint = false;
		map[26][20].picetype = north;
		map[26][20].playerison = false;
		map[26][20].endpoint = false;
		map[26][21].picetype = north;
		map[26][21].playerison = false;
		map[26][21].endpoint = false;
		map[26][22].picetype = north;
		map[26][22].playerison = false;
		map[26][22].endpoint = false;
		map[26][23].picetype = north;
		map[26][23].playerison = false;
		map[26][23].endpoint = false;
		map[26][24].picetype = north;
		map[26][24].playerison = false;
		map[26][24].endpoint = false;
		map[26][25].picetype = north;
		map[26][25].playerison = false;
		map[26][25].endpoint = false;
		map[26][26].picetype = north;
		map[26][26].playerison = false;
		map[26][26].endpoint = false;
		map[26][27].picetype = north;
		map[26][27].playerison = false;
		map[26][27].endpoint = false;
		map[26][28].picetype = north;
		map[26][28].playerison = false;
		map[26][28].endpoint = false;
		map[26][29].picetype = southeast;
		map[26][29].playerison = false;
		map[26][29].endpoint = false;
		map[27][0].picetype = northeast;
		map[27][0].playerison = false;
		map[27][0].endpoint = false;
		map[27][1].picetype = north;
		map[27][1].playerison = false;
		map[27][1].endpoint = false;
		map[27][2].picetype = north;
		map[27][2].playerison = false;
		map[27][2].endpoint = false;
		map[27][3].picetype = north;
		map[27][3].playerison = false;
		map[27][3].endpoint = false;
		map[27][4].picetype = north;
		map[27][4].playerison = false;
		map[27][4].endpoint = false;
		map[27][5].picetype = north;
		map[27][5].playerison = false;
		map[27][5].endpoint = false;
		map[27][6].picetype = north;
		map[27][6].playerison = false;
		map[27][6].endpoint = false;
		map[27][7].picetype = north;
		map[27][7].playerison = false;
		map[27][7].endpoint = false;
		map[27][8].picetype = north;
		map[27][8].playerison = false;
		map[27][8].endpoint = false;
		map[27][9].picetype = north;
		map[27][9].playerison = false;
		map[27][9].endpoint = false;
		map[27][10].picetype = north;
		map[27][10].playerison = false;
		map[27][10].endpoint = false;
		map[27][11].picetype = north;
		map[27][11].playerison = false;
		map[27][11].endpoint = false;
		map[27][12].picetype = north;
		map[27][12].playerison = false;
		map[27][12].endpoint = false;
		map[27][13].picetype = north;
		map[27][13].playerison = false;
		map[27][13].endpoint = false;
		map[27][14].picetype = north;
		map[27][14].playerison = false;
		map[27][14].endpoint = false;
		map[27][15].picetype = north;
		map[27][15].playerison = false;
		map[27][15].endpoint = false;
		map[27][16].picetype = north;
		map[27][16].playerison = false;
		map[27][16].endpoint = false;
		map[27][17].picetype = north;
		map[27][17].playerison = false;
		map[27][17].endpoint = false;
		map[27][18].picetype = north;
		map[27][18].playerison = false;
		map[27][18].endpoint = false;
		map[27][19].picetype = north;
		map[27][19].playerison = false;
		map[27][19].endpoint = false;
		map[27][20].picetype = north;
		map[27][20].playerison = false;
		map[27][20].endpoint = false;
		map[27][21].picetype = north;
		map[27][21].playerison = false;
		map[27][21].endpoint = false;
		map[27][22].picetype = north;
		map[27][22].playerison = false;
		map[27][22].endpoint = false;
		map[27][23].picetype = north;
		map[27][23].playerison = false;
		map[27][23].endpoint = false;
		map[27][24].picetype = north;
		map[27][24].playerison = false;
		map[27][24].endpoint = false;
		map[27][25].picetype = north;
		map[27][25].playerison = false;
		map[27][25].endpoint = false;
		map[27][26].picetype = north;
		map[27][26].playerison = false;
		map[27][26].endpoint = false;
		map[27][27].picetype = north;
		map[27][27].playerison = false;
		map[27][27].endpoint = false;
		map[27][28].picetype = north;
		map[27][28].playerison = false;
		map[27][28].endpoint = false;
		map[27][29].picetype = southwest;
		map[27][29].playerison = false;
		map[27][29].endpoint = false;
		map[28][0].picetype = northwest;
		map[28][0].playerison = false;
		map[28][0].endpoint = false;
		map[28][1].picetype = north;
		map[28][1].playerison = false;
		map[28][1].endpoint = false;
		map[28][2].picetype = north;
		map[28][2].playerison = false;
		map[28][2].endpoint = false;
		map[28][3].picetype = north;
		map[28][3].playerison = false;
		map[28][3].endpoint = false;
		map[28][4].picetype = north;
		map[28][4].playerison = false;
		map[28][4].endpoint = false;
		map[28][5].picetype = north;
		map[28][5].playerison = false;
		map[28][5].endpoint = false;
		map[28][6].picetype = north;
		map[28][6].playerison = false;
		map[28][6].endpoint = false;
		map[28][7].picetype = north;
		map[28][7].playerison = false;
		map[28][7].endpoint = false;
		map[28][8].picetype = north;
		map[28][8].playerison = false;
		map[28][8].endpoint = false;
		map[28][9].picetype = north;
		map[28][9].playerison = false;
		map[28][9].endpoint = false;
		map[28][10].picetype = north;
		map[28][10].playerison = false;
		map[28][10].endpoint = false;
		map[28][11].picetype = north;
		map[28][11].playerison = false;
		map[28][11].endpoint = false;
		map[28][12].picetype = north;
		map[28][12].playerison = false;
		map[28][12].endpoint = false;
		map[28][13].picetype = north;
		map[28][13].playerison = false;
		map[28][13].endpoint = false;
		map[28][14].picetype = north;
		map[28][14].playerison = false;
		map[28][14].endpoint = false;
		map[28][15].picetype = north;
		map[28][15].playerison = false;
		map[28][15].endpoint = false;
		map[28][16].picetype = north;
		map[28][16].playerison = false;
		map[28][16].endpoint = false;
		map[28][17].picetype = north;
		map[28][17].playerison = false;
		map[28][17].endpoint = false;
		map[28][18].picetype = north;
		map[28][18].playerison = false;
		map[28][18].endpoint = false;
		map[28][19].picetype = north;
		map[28][19].playerison = false;
		map[28][19].endpoint = false;
		map[28][20].picetype = north;
		map[28][20].playerison = false;
		map[28][20].endpoint = false;
		map[28][21].picetype = north;
		map[28][21].playerison = false;
		map[28][21].endpoint = false;
		map[28][22].picetype = north;
		map[28][22].playerison = false;
		map[28][22].endpoint = false;
		map[28][23].picetype = north;
		map[28][23].playerison = false;
		map[28][23].endpoint = false;
		map[28][24].picetype = north;
		map[28][24].playerison = false;
		map[28][24].endpoint = false;
		map[28][25].picetype = north;
		map[28][25].playerison = false;
		map[28][25].endpoint = false;
		map[28][26].picetype = north;
		map[28][26].playerison = false;
		map[28][26].endpoint = false;
		map[28][27].picetype = north;
		map[28][27].playerison = false;
		map[28][27].endpoint = false;
		map[28][28].picetype = north;
		map[28][28].playerison = false;
		map[28][28].endpoint = false;
		map[28][29].picetype = southeast;
		map[28][29].playerison = false;
		map[28][29].endpoint = false;
		map[29][0].picetype = north;
		map[29][0].playerison = false;
		map[29][0].endpoint = true;
		map[29][1].picetype = north;
		map[29][1].playerison = false;
		map[29][1].endpoint = false;
		map[29][2].picetype = north;
		map[29][2].playerison = false;
		map[29][2].endpoint = false;
		map[29][3].picetype = north;
		map[29][3].playerison = false;
		map[29][3].endpoint = false;
		map[29][4].picetype = north;
		map[29][4].playerison = false;
		map[29][4].endpoint = false;
		map[29][5].picetype = north;
		map[29][5].playerison = false;
		map[29][5].endpoint = false;
		map[29][6].picetype = north;
		map[29][6].playerison = false;
		map[29][6].endpoint = false;
		map[29][7].picetype = north;
		map[29][7].playerison = false;
		map[29][7].endpoint = false;
		map[29][8].picetype = north;
		map[29][8].playerison = false;
		map[29][8].endpoint = false;
		map[29][9].picetype = north;
		map[29][9].playerison = false;
		map[29][9].endpoint = false;
		map[29][10].picetype = north;
		map[29][10].playerison = false;
		map[29][10].endpoint = false;
		map[29][11].picetype = north;
		map[29][11].playerison = false;
		map[29][11].endpoint = false;
		map[29][12].picetype = north;
		map[29][12].playerison = false;
		map[29][12].endpoint = false;
		map[29][13].picetype = north;
		map[29][13].playerison = false;
		map[29][13].endpoint = false;
		map[29][14].picetype = north;
		map[29][14].playerison = false;
		map[29][14].endpoint = false;
		map[29][15].picetype = north;
		map[29][15].playerison = false;
		map[29][15].endpoint = false;
		map[29][16].picetype = north;
		map[29][16].playerison = false;
		map[29][16].endpoint = false;
		map[29][17].picetype = north;
		map[29][17].playerison = false;
		map[29][17].endpoint = false;
		map[29][18].picetype = north;
		map[29][18].playerison = false;
		map[29][18].endpoint = false;
		map[29][19].picetype = north;
		map[29][19].playerison = false;
		map[29][19].endpoint = false;
		map[29][20].picetype = north;
		map[29][20].playerison = false;
		map[29][20].endpoint = false;
		map[29][21].picetype = north;
		map[29][21].playerison = false;
		map[29][21].endpoint = false;
		map[29][22].picetype = north;
		map[29][22].playerison = false;
		map[29][22].endpoint = false;
		map[29][23].picetype = north;
		map[29][23].playerison = false;
		map[29][23].endpoint = false;
		map[29][24].picetype = north;
		map[29][24].playerison = false;
		map[29][24].endpoint = false;
		map[29][25].picetype = north;
		map[29][25].playerison = false;
		map[29][25].endpoint = false;
		map[29][26].picetype = north;
		map[29][26].playerison = false;
		map[29][26].endpoint = false;
		map[29][27].picetype = north;
		map[29][27].playerison = false;
		map[29][27].endpoint = false;
		map[29][28].picetype = north;
		map[29][28].playerison = false;
		map[29][28].endpoint = false;
		map[29][29].picetype = southwest;
		map[29][29].playerison = false;
		map[29][29].endpoint = false;
		blankstart();
	}
	void crandomstart()
	{
		clearmap();
		randommaze();
		while (!compleatiblemaze())
		{
			rerandommaze();
		}
		int a = getbestmoves();
		cout<<"this maze can be compleated in "<<a<<"moves"<<endl;
		blankstart();
	}
	void savefile()
	{
		ofstream mapsave;
		mapsave.open("new.map");
		for (int i = 0; i < mazesize; i++)
		{
			for (int j = 0; j < mazesize;j++)
			{
				mapsave<<map[i][j].picetype<<endl;
				mapsave<<map[i][j].playerison<<endl;
				mapsave<<map[i][j].endpoint<<endl;
			}
		}
		mapsave.close();
		cout<<"file saved"<<endl;
	}
}
int main()
{
	int a = EM_ASM_INT_V(
	{
		if (clicedclear = true)
		return 1;
		else if (clickedblank = true)
			return 2;
		else if (clickededit = true)
			return 3;
		else if (clickedpreset1 = true)
			return 4;
		else
			return 0;
	});
	if (a == 1)
	{
		clearmap();
	}
	if (a == 2)
	{
		blankstart();
	}
	if (a == 3)
	{
		editmode();
	}
	if (a == 4)
	{
		preset1();
	}
	if (a == 5)
	{
		crandomstart();
	}
	if (a == 6)
	{
		startall();
	}
	return 0;
}
//-s EXPORTED_FUNCTIONS='["_startall", "startall"]'
maze::maze()
{
	playerison = false;
	endpoint = false;
	hasbeing = false;
	bestpath = false;
	nonreatchable = false;
	picetype = all;
	distance = 10000;
}
maze::~maze()
{
	playerison = false;
	endpoint = false;
	hasbeing = false;
	bestpath = false;
	nonreatchable = false;
	picetype = all;
	distance = 10000;
}
void maze::print(const int x, const int y, const int size, const int linethicness)
{
    if (playerison)
    {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0x00, 0xFF, 0x00 ));
		hasbeing = true;
    }
    else if (endpoint)
    {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0x00, 0x00, 0xFF ));
    }
	else if (bestpath)
	{
		SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0xFF, 0xA5, 0x00 ));
	}
	else if (hasbeing)
	{
		SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0xFF, 0x00, 0x00 ));
	}
	else if (nonreatchable)
	{
		SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0x69, 0x69, 0x69 ));
	}
	else
	{
		SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0xF2, 0xF2, 0xF0 ));
	}
    switch (picetype)
    {
        case east:{
            SDL_Rect q = {x + 0, y + 0, size, linethicness};
            SDL_Rect b = {x + 0, y + size - linethicness, size, linethicness};
            SDL_FillRect(screen, &q,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
			SDL_FillRect(screen, &b,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case eastwastnorth:{
            SDL_Rect f = {x + 0, y + size - linethicness, size, linethicness};
			SDL_FillRect(screen, &f,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case eastwestsouth:{
            SDL_Rect g = {x + 0, y + 0, size, linethicness};
			SDL_FillRect(screen, &g,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case north:{
            SDL_Rect c = {x + 0, y + 0, linethicness, size};
            SDL_Rect d = {x + size - linethicness, y + 0, linethicness, size};
            SDL_FillRect(screen, &c,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
			SDL_FillRect(screen, &d,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case northeast:{
            SDL_Rect h = {x + 0, y + 0, linethicness, size};
            SDL_Rect i = {x + 0, y + 0, size, linethicness};
            SDL_FillRect(screen, &h,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
			SDL_FillRect(screen, &i,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case northwest:{
            SDL_Rect j = {x + size - linethicness, y + 0, linethicness, size};
            SDL_Rect k = {x + 0, y + 0, size, linethicness};
            SDL_FillRect(screen, &j,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
			SDL_FillRect(screen, &k,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case southeast:{
            SDL_Rect l = {x + 0, y + size - linethicness, size, linethicness};
            SDL_Rect m = {x + 0, y + 0, linethicness, size};
            SDL_FillRect(screen, &l,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
			SDL_FillRect(screen, &m,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case southwest:{
            SDL_Rect n = {x + 0, y + size - linethicness, size, linethicness};
            SDL_Rect o = {x + size - linethicness, y + 0, linethicness, size};
            SDL_FillRect(screen, &n,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
			SDL_FillRect(screen, &o,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
        case noenterable:{
            SDL_Rect p = {x + 0, y + 0, size, size};
			SDL_FillRect(screen, &p,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));}
            break;
    }
}
