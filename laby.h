#ifndef _LABY_H_
#define _LABY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <ctype.h>

//The labyMap structure to be used
typedef struct{
    unsigned int levelId; //identifier of the level
    unsigned int width; //number of cells (counting empty ones)
    int* numcontent; //array of the numerical values in order from top left to down right
    char* opcontent; //array of the operators in order from top left to down right
    unsigned int position; //current position
    int startingScore; //score of the starting position
    int goalScore; //target score
} labyMap;

//Constant values to be used
enum direction {UP, DOWN, RIGHT, LEFT, DIRUNDEFINED};
enum operation {PLUS='+', MINUS='-', MULTIPLY='*', DIVIDE='/', UNDEFINED='?'};

int getActualValue(labyMap * m);

labyMap * getLabyMapFromFile(FILE * f);

void drawLabyMap (labyMap * m);

enum operation getOperation(labyMap * m, enum direction d);

bool applyOperation (labyMap * m, enum direction d);

enum direction * resolve (labyMap * map);

#endif // _LABY_H_