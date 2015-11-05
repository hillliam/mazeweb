// Copyright Liam hill 2013
#include "maze.h"

maze::maze() {

    playerison = false;
    endpoint = false;
    hasbeing = false;
    bestpath = false;
    nonreatchable = false;
    picetype = all;
    distance = 10000;
}

maze::~maze() {

    playerison = false;
    endpoint = false;
    hasbeing = false;
    bestpath = false;
    nonreatchable = false;
    picetype = all;
    distance = 10000;
}

void maze::print(SDL_Surface *screen, const int x, const int y, const int size, const int linethicness) {
    if (playerison) {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00));
        hasbeing = true;
    } else if (endpoint) {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF));
    } else if (bestpath) {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0xFF, 0xA5, 0x00));
    } else if (hasbeing) {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00));
    } else if (nonreatchable) {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0x69, 0x69, 0x69));
    } else {
        SDL_Rect p = {x + 0, y + 0, size, size};
        SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0xF2, 0xF2, 0xF0));
    }
    switch (picetype) {
        case east:
        {
            SDL_Rect q = {x + 0, y + 0, size, linethicness};
            SDL_Rect b = {x + 0, y + size - linethicness, size, linethicness};
            SDL_FillRect(screen, &q, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
            SDL_FillRect(screen, &b, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case eastwastnorth:
        {
            SDL_Rect f = {x + 0, y + size - linethicness, size, linethicness};
            SDL_FillRect(screen, &f, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case eastwestsouth:
        {
            SDL_Rect g = {x + 0, y + 0, size, linethicness};
            SDL_FillRect(screen, &g, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case north:
        {
            SDL_Rect c = {x + 0, y + 0, linethicness, size};
            SDL_Rect d = {x + size - linethicness, y + 0, linethicness, size};
            SDL_FillRect(screen, &c, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
            SDL_FillRect(screen, &d, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case northeast:
        {
            SDL_Rect h = {x + 0, y + 0, linethicness, size};
            SDL_Rect i = {x + 0, y + 0, size, linethicness};
            SDL_FillRect(screen, &h, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
            SDL_FillRect(screen, &i, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case northwest:
        {
            SDL_Rect j = {x + size - linethicness, y + 0, linethicness, size};
            SDL_Rect k = {x + 0, y + 0, size, linethicness};
            SDL_FillRect(screen, &j, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
            SDL_FillRect(screen, &k, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case southeast:
        {
            SDL_Rect l = {x + 0, y + size - linethicness, size, linethicness};
            SDL_Rect m = {x + 0, y + 0, linethicness, size};
            SDL_FillRect(screen, &l, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
            SDL_FillRect(screen, &m, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case southwest:
        {
            SDL_Rect n = {x + 0, y + size - linethicness, size, linethicness};
            SDL_Rect o = {x + size - linethicness, y + 0, linethicness, size};
            SDL_FillRect(screen, &n, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
            SDL_FillRect(screen, &o, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
        case noenterable:
        {
            SDL_Rect p = {x + 0, y + 0, size, size};
            SDL_FillRect(screen, &p, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
            break;
    }
}

void maze::setplayerison(bool playeron) {
    playerison = playeron;
}

bool maze::getplayerison() {
    return playerison;
}

void maze::setendpoint(bool end) {
    endpoint = end;
}

bool maze::getendpoint() {
    return endpoint;
}

void maze::sethasbeing(bool being) {
    hasbeing = being;
}

bool maze::gethasbeing() {
    return hasbeing;
}

void maze::setbestpath(bool best) {
    bestpath = best;
}

bool maze::getbestpath() {
    return bestpath;
}

void maze::setnonreatchable(bool notreatch) {
    nonreatchable = notreatch;
}

bool maze::getnonreatchable() {
    return nonreatchable;
}

void maze::setdistance(int steps) {
    distance = steps;
}

int maze::getdistance() {
    return distance;
}

maze_pices maze::getpicetype() {
    return picetype;
}

void maze::setpicetype(maze_pices item) {
    picetype = item;
}