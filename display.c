/*
   Created by Cassidy on 02/12/2022.
*/
#include <stdio.h>
#include "main.h"
#include <stdlib.h>

const char* crNames[4] = {"Blank", "Lettuce", "Slug", "Frog"};
const char* moveTypes[7] = {"moved to", "appeared at", "stood still", "went away somewhere...", "reigns supreme!", "is stuck...", "wanted to appear!"};

/** The clearScreen function clears the screen depending on the given OS **/
void clearScreen(){
#if defined(_WIN32)
    system("cls"); //Windows
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
    system("clear");
#else
    printf("\e[1;1H\e[2J");
#endif
}

/** the getName function returns the string name of a species enum
 * @param species - the species enum
 * @return - the name of the species
 */
const char* getName(int species) {
    switch (species) {
        case BLANK: return crNames[0];
        case LETTUCE: return crNames[1];
        case SLUG: return crNames[2];
        case FROG: return crNames[3];
        default: return "Error";
    }
}

/** the printHbed function outputs the high bed array as a grid
 * @param g - the game struct */
void printHBed(game* g){
    int hbedSize = g->pr.i.paramStructInt.hbedsize;
    clearScreen();
    printf("   ");
    for (int i = 0; i <hbedSize; i++) { /*print top row*/
        printf((i > 9) ? "%i " : "%i  ", i); /*number spacing (fails for numbers above 99 or negatives)*/
    }
    for (int i = 0; i <hbedSize; i++){
        printf("\n");
        printf((i > 9) ? "%i " : "%i  ", i);

        for (int j = 0; j < hbedSize; j++) {
            switch (g->hbed[j][i].occupier.species) {
                case LETTUCE: printf("O  "); break;
                case SLUG: printf((g->hbed[j][i].occupier.age >= g->pr.i.paramStructInt.matureSlug) ? "S  " : "s  "); break;
                case FROG: printf((g->hbed[j][i].occupier.age >= g->pr.i.paramStructInt.matureFrog) ? "F  " : "f  "); break;
                default: printf("   ");
            }
        }
    } printf("\n");
}

/** the printPos function outputs a message involving a creature and a position.
 *  the printSpecies function does the same, without a position value
 * @param species - the game struct
 * @param move - the enum value of the type of move, corrosponding to the moveTypes[] array
 * @param p - the position value */
void printPos(int species, int move, pos p){ printf("A %s %s (%i,%i)\n", getName(species), moveTypes[move], p.row,p.column); }
void printSpecies(int species, int move){ printf("A %s %s\n", getName(species), moveTypes[move]); }

#ifdef DEBUG
    void printRead_float(int species, float fl){
        printf("%s growth is %fl\n", getName(species), fl);
    }

    void printRead_int(int i, int x){
        const char* prNames[6] = {"slug maturity", "frog maturity", "slug lifespan", "frog lifespan", "frog hunger", "frog vision"};
        printf("%s is %i\n", prNames[i], x);
}
#endif
