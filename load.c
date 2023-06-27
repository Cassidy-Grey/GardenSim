/*
   Created by Cassidy on 02/12/2022.
*/
#include "load.h"
#include <stdlib.h>
#ifdef STACKDEBUG
    #include <string.h>
#endif

/** The isEmpty function tests whether the file has data left.
 * @param stream - the data stream
 * @return - whether the file is empty */
bool isEmpty(FILE* stream){
    int c;
    c = fgetc(stream);
    if (c == EOF) { return true; }
    else{ ungetc(c, stream); return false; }
}

/** the getFile function gets the name of a config file from the user
 * @param target - the address to write the filename to */
void getFile(char* target){
    printf("What is the data file called?\n"); /*get filename*/
    fflush(stdin);
    scanf("%31[^\n]", target);
}

/** The getStream function opens a config file for reading
 * @return - the data stream */
FILE* getStream(){
    char* target = malloc(31 * sizeof(char)); /*allow space for up to 31 characters in a filename*/
    FILE* stream = NULL;
    do {
        getFile(target);
        stream = fopen(target, "r"); /*open file*/
        if (stream == NULL){ printf("File does not exist!\n"); }
    } while (stream == NULL); /*catch if file failed to open*/

    free(target);
    return stream;
}

/** The get_(type) reads data of the defined type
 * @param stream  the data stream
 * @param data[] - the array the data will be stored in
 * @param amount - the amount of data to be read
 * @param format - the format specifier for the data in the config file */
#define define_get(type) \
void get_##type(FILE* stream, type data[], int amount, char* format) { \
    type temp; \
    char st[22]; \
\
    for (int j = 0; j < amount; j++) { \
        fscanf(stream, format, st, &temp, st);  \
        data[j] = temp; \
    } \
}

/** The assign_(type) function assigns read data to a given variable
 * @param stream  the data stream
 * @param config - the struct holding the config data
 * @param amount - the amount of data to be read
 * @param format - the regex format of the data in the config file
 * @param cr - the number of the current creature in the array */
#define define_assign(label, type, store)\
void assign_##label(FILE* stream, data* config, int amount, char* format, int cr){ \
    type dat[amount]; \
    get_##type(stream, dat, amount, format); \
\
    for (int i = 0; i < amount; i++) { \
        config->store = dat[i]; \
    } \
} \

define_get(int)
define_get(float)
define_assign(paramf, float, pr_float.paramArray_float[i])
define_assign(parami, int, pr_int.paramArray_int[i])
define_assign(coords, int, creatures[cr].coords[i])
define_assign(species, int, creatures[cr].cr.species)
define_assign(age, int, creatures[cr].cr.age)
define_assign(hunger, int, creatures[cr].cr.hunger)

/** The getFromFile function reads data from the config file
 * @param config - the struct to hold the config data */
data getFromFile(data* config){
    FILE* stream = getStream();
    assign_paramf(stream, config, FLOATS, "%[^ ] %f %[\n]",0);
    assign_parami(stream, config, INTS, "%[^ ] %i %[\n]",0);

    int cr = 0;
    while (!isEmpty(stream)) { //collect Creature Data
        assign_coords(stream, config, 2, "%[^0-9] %i", cr);
        assign_species(stream, config, 1, "%[^ ] %c", cr); /*no catch for invalid species*/
        assign_age(stream, config, 1, "%[^0-9] %i", cr);
        assign_hunger(stream, config, 1, "%[^0-9] %i", cr);

        int di[2]; //store direction data
        get_int(stream, di, 2, "%c %c"); /*get direction*/
        switch (di[0]) { //assign direction
            case 'N': config->creatures[cr].cr.direction = NORTH; break;
            case 'E': config->creatures[cr].cr.direction = EAST; break;
            case 'W': config->creatures[cr].cr.direction = WEST; break;
            case 'S': config->creatures[cr].cr.direction = (di[1] == 'U' ?  SOUTH : STILL); break;
            default: printf("Invalid Direction!");
        }
        char st[31]; /*space to skip up to 31 characters*/
        fscanf(stream, "%[^\n]", st); /*skip extraneous characters*/
#ifdef DEBUG
        printf("Printing creature data...\n");
        printf("co-ordinate x is %i\n", config->creatures[cr].coords[0]);
        printf("co-ordinate y is %i\n", config->creatures[cr].coords[1]);
        printf("species is %u\n", config->creatures[cr].cr.species);
        printf("age is %i\n", config->creatures[cr].cr.age);
        printf("hunger is %i\n", config->creatures[cr].cr.hunger);
        printf("direction is %u\n", config->creatures[cr].cr.direction);
#endif
        cr++;
    }
#ifdef DEBUG
    printf(LETTUCE, config->pr_float.paramArray_float[0]);
    printRead_float(SLUG, config->pr_float.paramArray_float[1]);
    printRead_float(FROG, config->pr_float.paramArray_float[2]);
    for(int i = 0; i < 6; i++) {
        printRead_int(i, config->pr_int.paramArray_int[i]);
    }
    fflush(stdin);
    getchar();
#endif
    return *config;
}

/** The addToHbed function writes the config data to the hbed array
 * @param hbed - the array holding the high bed
 * @param creatures[] - an array of creatures to be written and their positions */
void addToHbed(game* g,creatureData creatures[]){
    int hbedSize = g->pr.i.paramStructInt.hbedsize;

    for (int i = 0; i < hbedSize; i++){ /*initialise all to blank*/
        for (int j = 0; j < hbedSize; j++){ g->hbed[i][j].occupier.species = BLANK; }
    }

    int i = 0;
    int sp = creatures[i].cr.species;

    while (sp == LETTUCE || sp == SLUG || sp == FROG){ /*must check for exact values. if checking for 'not blank', could attempt to write to uninitialised places.*/
        g->hbed[creatures[i].coords[0]][creatures[i].coords[1]].occupier = creatures[i].cr;
        i++;
        sp = creatures[i].cr.species;
    }
}
