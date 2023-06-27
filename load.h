/*
   Created by Cassidy on 11/11/2022.
   Contains header data for use in loading data from config files
*/

#ifndef UNTITLED_LOAD_H
#define UNTITLED_LOAD_H
/*includes*/
#include <stdio.h>
#include "main.h"
/*parameters*/
#define MAXDATA 32 /*The largest amount of values to be read*/
#define FLOATS 3 /*The amount of float values in the data file*/
#define INTS 7 /*The amount of int values in the data file*/

/*structs*/
typedef struct creatureData { /*to store the line of creature data*/
    int coords[2];
    creature cr;
} creatureData;

typedef struct data{ /*to hold the config data taken from file*/
    paramsFloat pr_float;
    paramsInt pr_int;
    creatureData creatures[MAXDATA];
} data;

/*functions*/
bool isEmpty(FILE* stream);
data getFromFile(data* config);
void addToHbed(game* g, creatureData creatures[]);

#endif /*UNTITLED_LOAD_H*/
