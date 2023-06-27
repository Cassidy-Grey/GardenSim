/*
   Created by Cassidy on 11/11/2022.
*/
/*#define DEBUG*/ /*for testing*/

#ifndef UNTITLED_MAIN_H
#define UNTITLED_MAIN_H
/*includes*/
#include <stdbool.h>

/*parameters*/
#define SLUG_VISION 1

/*enums*/
typedef enum {BLANK, LETTUCE = 76, SLUG = 83, FROG = 70} species; /*numbers = ascii values*/
typedef enum {NORTH, EAST, SOUTH, WEST, STILL} direction;
typedef enum {MOVE, APPEAR, STOOD, AWAY, END, STUCK, SPAWNFAIL} moves;
typedef enum {visionFROG = 5, visionSLUG = 7} paramIntNames;

/*structs*/
typedef union paramsFloat{
    struct paramStruct_float{
        float growLettuce;
        float reproduceSlug;
        float reproduceFrog;
    } paramStructFloat;
    float paramArray_float[3];
} paramsFloat;

typedef union paramsInt{
    struct paramStruct_int{
        int matureSlug;
        int matureFrog;
        int lifespanSlug;
        int lifespanFrog;
        int hungryFrog;
        int visionFrog;
        int hbedsize;
        int visionSlug;
    } paramStructInt;
    int paramArray_int[8];
} paramsInt;

typedef struct params {
    union paramsFloat f;
    union paramsInt i;
} params;

typedef struct creatureStruct { /*to store the line of creature data*/
    species species;
    int age;
    int hunger;
    direction direction;
} creature;

typedef struct cellStruct {
    creature occupier;
} cell;

typedef struct posStruct {
    int row;
    int column;
} pos, offset;

typedef struct gameStruct {
    params pr;
    char mode;
    cell** hbed;
} game;

/*external definitions*/
extern const offset vector[]; /*defined in move.c*/
extern const char* crNames[4]; /*defined in display.c*/

/*functions*/
void act(game* g, pos p);

bool crMove(game* g, pos p, int speed); /*move*/
pos inRange(game* g, pos p, int range, species target);
bool checkSpace(game* g, pos p);
void emptySpace(game* g, pos p);

void printHBed(game* g); /*display*/
void printPos(int species, int move, pos p);
void printSpecies(int species, int move);
void clearScreen();

#ifdef DEBUG
void printRead_int(int species, int i);
void printRead_float(int species, float f);
#endif




#endif /*UNTITLED_MAIN_H*/
