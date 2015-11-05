// mazeweb.cpp : Defines the entry point for the console application.
// Copyright Liam hill 2013

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

bool ValidMoves(const int startx, const int starty, const int tox, const int toy) {
    if (tox < 0 || tox > mazesize - 1 || toy < 0 || toy > mazesize - 1) {
        return false;
    }
    switch (map[startx][starty].getpicetype()) {
        case all:
        case north:
        case eastwastnorth:
        case southeast:
        case southwest:
            if (tox == startx && toy == starty - 1) {
                switch (map[tox][toy].getpicetype()) {
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
    switch (map[startx][starty].getpicetype()) {
        case all:
        case east:
        case eastwestsouth:
        case eastwastnorth:
        case northwest:
        case southwest:
            if (tox == startx - 1 && toy == starty) {
                switch (map[tox][toy].getpicetype()) {
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
    switch (map[startx][starty].getpicetype()) {
        case all:
        case north:
        case eastwestsouth:
        case northeast:
        case northwest:
            if (tox == startx && toy == starty + 1) {
                switch (map[tox][toy].getpicetype()) {
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
    switch (map[startx][starty].getpicetype()) {
        case all:
        case east:
        case eastwestsouth:
        case eastwastnorth:
        case northeast:
        case southeast:
            if (tox == startx + 1 && toy == starty) {
                switch (map[tox][toy].getpicetype()) {
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

int getmovesx(int startx, int direction) {
    switch (direction) {
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

int getmovesy(int starty, int direction) {
    switch (direction) {
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

void Pathfind() {
    int startx = 0;
    int starty = 0;
    bool found = false;
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            if (map[i][j].getplayerison()) {
                startx = i;
                starty = j;
                found = true;
                break;
            }
        }
    }
    if (found) {
        map[startx][starty].setdistance(0);
        while (true) {
            bool madeProgress = false;
            for (int i = 0; i < mazesize; i++) {
                for (int j = 0; j < mazesize; j++) {
                    int x = i;
                    int y = j;
                    int passHere = map[x][y].getdistance();
                    for (int a = 1; a < 5; a++) {
                        int newx = getmovesx(x, a);
                        int newy = getmovesy(y, a);
                        if (ValidMoves(x, y, newx, newy)) {
                            int newPass = passHere + 1;
                            if (map[newx][newy].getdistance() > newPass) {
                                map[newx][newy].setdistance(newPass);
                                madeProgress = true;
                            }
                        }
                    }
                }
            }
            if (!madeProgress) {
                break;
            }
        }
    }
}

int getbestmoves() {
    Pathfind();
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            if (map[i][j].getendpoint()) {
                return map[i][j].getdistance();
            }
        }
    }
    return 0;
}

bool compleatiblemaze() {
    Pathfind();
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            if (map[i][j].getendpoint() && map[i][j].getdistance() != 10000) {
                return true;
            }
        }
    }
    return false;
}

void rerandommaze() {
    int lowerBound = 0;
    int upperBound = 30;
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            if (map[i][j].getdistance() == 10000) {
                int randnum = (lowerBound + (int) (emscripten_random()* 100) % (10 - lowerBound));
                map[i][j].setpicetype((maze_pices) randnum);
                if (map[i][j].getpicetype() == noenterable && map[i][j].getplayerison()) {
                    map[i][j].setpicetype(all);
                }
                if (map[i][j].getpicetype() == noenterable && map[i][j].getendpoint()) {
                    map[i][j].setpicetype(all);
                }
            }
        }
    }
}

void showbestpath() {
    bool found = false;
    int exitx = 0;
    int exity = 0;
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            if (map[i][j].getendpoint()) {
                exitx = i;
                exity = j;
                found = true;
            }
        }
    }
    while (found) {
        int lowestx = 0;
        int lowesty = 0;
        int lowest = 10000;
        for (int i = 1; i < 5; i++) {
            int newx = getmovesx(exitx, i);
            int newy = getmovesy(exity, i);
            if (ValidMoves(exitx, exity, newx, newy)) {
                int count = map[newx][newy].getdistance();
                if (count < lowest) {
                    lowest = count;
                    lowestx = newx;
                    lowesty = newy;
                }
            }
        }
        if (lowest != 10000) {
            map[lowestx][lowesty].setbestpath(true);
            exitx = lowestx;
            exity = lowesty;
        } else {
            break;
        }
        if (exitx == beginx & exity == beginy) {
            break;
        }
    }
}

void shownonreatchable() {
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            if (map[i][j].getdistance() == 10000) {
                map[i][j].setnonreatchable(true);
            }
        }
    }
}

void movemaze(const int direction) {
    int x = 0;
    int y = 0;
    bool canmove = false;
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            if (map[i][j].getplayerison()) {
                x = i;
                y = j;
                canmove = true;
            }
            if (map[i][j].getplayerison() & map[i][j].getendpoint()) {
                canmove = false;
                if (win == false) {
                    shownonreatchable();
                    win = true;
                    if (moves > bestmoves) {
                        EM_ASM_(window.alert('you have compleated the maze in ' + $0 + ' try again and acheave ' + $1), moves, bestmoves);
                        cout << "you have compleated the maze in " << moves << " try again and acheave " << bestmoves << endl;
                        showbestpath();
                    } else {
                        EM_ASM_(window.alert('you have compleated the maze in ' + $0), moves);
                        cout << "you have compleated the maze in " << moves << endl;
                    }
                } else {
                    EM_ASM_(window.alert('you have compleated the maze in ' + $0 + ' try a new maze'), moves);
                    cout << "you have compleated the maze in " << moves << " try a new maze" << endl;
                }
            }
        }
    }
    if (canmove) {
        if (direction == 1)// up
        {
            if (y != 0) {
                switch (map[x][y].getpicetype()) {
                    case all:
                    case north:
                    case eastwastnorth:
                    case southeast:
                    case southwest:
                        switch (map[x][y - 1].getpicetype()) {
                            case all:
                            case north:
                            case eastwestsouth:
                            case northeast:
                            case northwest:
                                map[x][y].setplayerison(false);
                                map[x][y - 1].setplayerison(true);
                                moves++;
                                break;
                        }
                        break;
                }

            }
        }
        if (direction == 2)//left
        {
            if (x != 0) {
                switch (map[x][y].getpicetype()) {
                    case all:
                    case east:
                    case eastwestsouth:
                    case eastwastnorth:
                    case northwest:
                    case southwest:
                        switch (map[x - 1][y].getpicetype()) {
                            case all:
                            case east:
                            case eastwastnorth:
                            case eastwestsouth:
                            case southeast:
                            case northeast:
                                map[x][y].setplayerison(false);
                                map[x - 1][y].setplayerison(true);
                                moves++;
                                break;
                        }
                        break;
                }
            }
        }
        if (direction == 3)//down
        {
            if (y != 30 - 1) {
                switch (map[x][y].getpicetype()) {
                    case all:
                    case north:
                    case eastwestsouth:
                    case northeast:
                    case northwest:
                        switch (map[x][y + 1].getpicetype()) {
                            case all:
                            case north:
                            case eastwastnorth:
                            case southeast:
                            case southwest:
                                map[x][y].setplayerison(false);
                                map[x][y + 1].setplayerison(true);
                                moves++;
                                break;
                        }
                        break;
                }
            }
        }
        if (direction == 4)//right
        {
            if (x != 30 - 1) {
                switch (map[x][y].getpicetype()) {
                    case all:
                    case east:
                    case eastwestsouth:
                    case eastwastnorth:
                    case northeast:
                    case southeast:
                        switch (map[x + 1][y].getpicetype()) {
                            case all:
                            case east:
                            case eastwastnorth:
                            case eastwestsouth:
                            case northwest:
                            case southwest:
                                map[x][y].setplayerison(false);
                                map[x + 1][y].setplayerison(true);
                                moves++;
                                break;
                        }
                        break;
                }
            }
        }
    }
}

void drawmaze() {
    //SDL_LockSurface(screen);
    int space = 25;
    int linethicness = 5;
    int locationx = 20;
    int locationy = 30;
    for (int i = 0; i < mazesize; i++) {
        locationy = 30;
        for (int j = 0; j < mazesize; j++) {
            locationy += space;
            map[i][j].print(screen, locationx, locationy, space, linethicness);
            if (map[i][j].getendpoint() && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space) {
                cout << "this is the location you have to reach" << endl;
            } else if (map[i][j].getplayerison() && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space) {
                cout << "you are here" << endl;
            } else if (map[i][j].getbestpath() && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space) {
                cout << "the way you should have gone" << endl;
            } else if (map[i][j].gethasbeing() && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space) {
                cout << "you where here" << endl;
            } else if (map[i][j].getnonreatchable() && mousex >= locationx && mousex <= locationx + space && mousey >= locationy && mousey <= locationy + space) {
                cout << "not reatchable" << endl;
            }

        }
        locationx += space;
    }
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, with, hight);
}

void randommaze() {
    int lowerBound = 0;
    int upperBound = 30;
    int rndValue1 = (lowerBound + (int) (emscripten_random()* 100) % (upperBound - lowerBound));
    int rndValue2 = (lowerBound + (int) (emscripten_random()* 100) % (upperBound - lowerBound));
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            map[i][j].setplayerison(false);
            map[i][j].setendpoint(false);
        }
    }
    map[rndValue1][rndValue2].setplayerison(true);
    rndValue1 = (lowerBound + (int) (emscripten_random()* 100) % (upperBound - lowerBound));
    rndValue2 = (lowerBound + (int) (emscripten_random()* 100) % (upperBound - lowerBound));
    map[rndValue1][rndValue2].setendpoint(true);
    for (int i = 0; i < mazesize; i++) {
        for (int j = 0; j < mazesize; j++) {
            int randnum = (lowerBound + (int) (emscripten_random()* 100) % (10 - lowerBound));
            map[i][j].setpicetype((maze_pices) randnum);
            if (map[i][j].getpicetype() == noenterable && map[i][j].getplayerison()) {
                map[i][j].setpicetype(all);
            }
            if (map[i][j].getpicetype() == noenterable && map[i][j].getendpoint()) {
                map[i][j].setpicetype(all);
            }
        }
    }
}

void getkinput() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
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

void getminput() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    mousex = x;
    mousey = y;
}

void geteditminput() {
    SDL_Event event;
    SDL_PollEvent(&event);
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int space = 25;
        int linethicness = 5;
        int locationx = 20;
        int locationy = 30;
        for (int i = 0; i < mazesize; i++) {
            locationy = 30;
            for (int j = 0; j < mazesize; j++) {
                locationy += space;
                if (x >= locationx && x <= locationx + space && y >= locationy && y <= locationy + space) {
                    int playison = EM_ASM_INT_V({
                        var player = document.getElementsByName('player');
                        if (player.checked) {
                            return 1;
                        } else {
                            return 0;
                        }
                    });
                    int exit = EM_ASM_INT_V({
                        var exit = document.getElementsByName('end');
                        if (exit.checked) {
                            return 1;
                        } else {
                            return 0;
                        }
                    });
                    int action = EM_ASM_INT_V({
                        var radios = document.getElementsByName('mapset');
                        for (var i = 0, length = radios.length; i < length; i++) {
                            if (radios[i].checked) {
                                return i;
                            }
                        }
                    });
                    map[i][j].setpicetype((maze_pices) action);
                    cout << "changed section " << i << " " << j << " to " << ((maze_pices) action) << "at " << x << " and" << y << endl;
                    if (playison == 1) {
                        for (int i2 = 0; i < mazesize; i++) {
                            for (int j2 = 0; j < mazesize; j++) {
                                map[i2][j2].setplayerison(false);
                            }
                        }
                        map[i][j].setplayerison(true);
                    } else if (exit == 1) {
                        for (int i2 = 0; i < mazesize; i++) {
                            for (int j2 = 0; j < mazesize; j++) {
                                map[i2][j2].setendpoint(false);
                            }
                        }
                        map[i][j].setendpoint(true);
                    }
                    break;
                }
            }
            locationx += space;
        }
    }
}

void looplogic() {

    getkinput();
    getminput();
    drawmaze();
    //numFrames++;
    //float fps = ( numFrames/(float)(SDL_GetTicks()) )*1000;
    //cout<<"the frames per second is at : "<< fps<<" and the total number of frames is : "<<numFrames<<endl;
}

void editloop() {
    geteditminput();
    drawmaze();
}
extern "C" {

    EMSCRIPTEN_KEEPALIVE void clearmap() {
        for (int i = 0; i < mazesize; i++) {
            for (int j = 0; j < mazesize; j++) {

                map[i][j].setplayerison(false);
                map[i][j].setendpoint(false);
                map[i][j].setbestpath(false);
                map[i][j].sethasbeing(false);
                map[i][j].setnonreatchable(false);
                map[i][j].setdistance(10000);
                map[i][j].setpicetype(all);
            }
        }
    }

    EMSCRIPTEN_KEEPALIVE void loadmap(vector<string> data) {
        int r = 0;
        for (int i = 0; i < mazesize; i++) {
            for (int j = 0; j < mazesize; j++) {
                if (data[r] == "north") {
                    map[i][j].setpicetype(north);
                } else if (data[r] == "east") {
                    map[i][j].setpicetype(east);
                } else if (data[r] == "all") {
                    map[i][j].setpicetype(north);
                } else if (data[r] == "eastwastnorth") {
                    map[i][j].setpicetype(eastwastnorth);
                } else if (data[r] == "eastwestsouth") {
                    map[i][j].setpicetype(eastwestsouth);
                } else if (data[r] == "noenterable") {
                    map[i][j].setpicetype(noenterable);
                } else if (data[r] == "southeast") {
                    map[i][j].setpicetype(southeast);
                } else if (data[r] == "southwest") {
                    map[i][j].setpicetype(southwest);
                } else if (data[r] == "northeast") {
                    map[i][j].setpicetype(northeast);
                } else if (data[r] == "northwest") {
                    map[i][j].setpicetype(northwest);
                }
                r++;
                if (data[r] == "true" || data[r] == "TRUE") {
                    map[i][j].setplayerison(true);
                } else {
                    map[i][j].setplayerison(false);
                }
                r++;
                if (data[r] == "true" || data[r] == "TRUE") {
                    map[i][j].setendpoint(true);
                } else {
                    map[i][j].setendpoint(false);
                }
                r++;
            }
        }
        moves = 0;
        cout << "begin" << endl;
        SDL_Init(SDL_INIT_VIDEO);
        atexit(SDL_Quit);
        screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
        if (screen == NULL) {

            cout << "no canvas" << endl;
        }
        drawmaze();
        cout << "start" << endl;
        //startTime = SDL_GetTicks();
        emscripten_cancel_main_loop();
        emscripten_set_main_loop(looplogic, 0, 0);
    }

    EMSCRIPTEN_KEEPALIVE void loadfile(string contence) {
        clearmap();
        cout << contence << endl;
        vector<string> list;
        string item = "";
        for (char a : contence) {
            if (a == '/n') {
                list.push_back(item);
            } else {

                item += a;
            }
        }
        loadmap(list);
    }

    EMSCRIPTEN_KEEPALIVE int startall() {
        clearmap();
        cout << "begin" << endl;
        SDL_Init(SDL_INIT_VIDEO);
        atexit(SDL_Quit);
        screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
        if (screen == NULL) {
            cout << "no canvas" << endl;
        }
        randommaze();
        drawmaze();
        cout << "start" << endl;
        bestmoves = getbestmoves();
        for (int i = 0; i < mazesize; i++) {
            for (int j = 0; j < mazesize; j++) {
                if (map[i][j].getplayerison()) {
                    beginx = i;
                    beginy = j;
                }
            }
        }
        //startTime = SDL_GetTicks();
        emscripten_cancel_main_loop();
        emscripten_set_main_loop(looplogic, 0, 0);

        return 0;
    }

    EMSCRIPTEN_KEEPALIVE int blankstart() {
        cout << "begin" << endl;
        SDL_Init(SDL_INIT_VIDEO);
        atexit(SDL_Quit);
        screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
        if (screen == NULL) {
            cout << "no canvas" << endl;
        }
        drawmaze();
        cout << "start" << endl;
        bestmoves = getbestmoves();
        for (int i = 0; i < mazesize; i++) {
            for (int j = 0; j < mazesize; j++) {
                if (map[i][j].getplayerison()) {
                    beginx = i;
                    beginy = j;
                }
            }
        }
        //startTime = SDL_GetTicks();
        emscripten_cancel_main_loop();
        emscripten_set_main_loop(looplogic, 0, 0);
        return 0;
    }

    EMSCRIPTEN_KEEPALIVE void editmode() {
        clearmap();
        cout << "begin" << endl;
        SDL_Init(SDL_INIT_VIDEO);
        atexit(SDL_Quit);
        screen = SDL_SetVideoMode(with, hight, bpp, SDL_SWSURFACE);
        if (screen == NULL) {

            cout << "no canvas" << endl;
        }
        drawmaze();
        cout << "start" << endl;
        emscripten_cancel_main_loop();
        emscripten_set_main_loop(editloop, 0, 0);
    }

    EMSCRIPTEN_KEEPALIVE void crandomstart() {
        clearmap();
        randommaze();
        while (!compleatiblemaze()) {

            rerandommaze();
        }
        int a = getbestmoves();
        cout << "this maze can be compleated in " << a << "moves" << endl;
        blankstart();
    }

    EMSCRIPTEN_KEEPALIVE void savefile() {
        ofstream mapsave;
        mapsave.open("new.map");
        for (int i = 0; i < mazesize; i++) {
            for (int j = 0; j < mazesize; j++) {

                mapsave << map[i][j].getpicetype() << endl;
                mapsave << map[i][j].getplayerison() << endl;
                mapsave << map[i][j].getendpoint() << endl;
            }
        }
        mapsave.close();
        cout << "file saved" << endl;
    }
}

int main() {
    int a = EM_ASM_INT_V({
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
    if (a == 1) {
        clearmap();
    }
    if (a == 2) {
        blankstart();
    }
    if (a == 3) {
        editmode();
    }
    if (a == 4) {
        //preset1();
    }
    if (a == 5) {
        crandomstart();
    }
    if (a == 6) {

        startall();
    }
    return 0;
}
