#ifndef main_h
#define main_h

#include <iostream>
#include <getopt.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstdlib>

using namespace std;

struct coord {
    int room;
    int spot;
};

struct listCoord {
    int row;
    int col;
    int room;
    char character;
};

struct option longOpts[] = {
    {"stack", no_argument, NULL, 's'},
    {"queue", no_argument, NULL, 'q'},
    {"output", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'}
};

// returns 1 if an input character is valid
bool validInput(char);

// returns 1 if list coordinate is valid
bool validListCoord(listCoord*, int, int);

// Returns 1 if a line is not a comment line
bool notComment(string);

// returns 0 if invalid, 1 if valid, 2 if portal, and 3 if ring
int neighbor(char);

// returns 1 if the space has not been seen before
// and can ber moved to
bool neverSeen(char);

// returns 1 if the space is a portal
bool isPortal(char);

// returns 10 if north, 11 if east, 12 if south, 13 if west,
// 14 to stop, or floor # if portal
int pathFinder(char);

#endif