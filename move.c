/*
Created by Cassidy on 02/12/2022.
*/
#include <stdlib.h>
#include "main.h"

const offset vector[] = {
        {0,-1},
        {1, 0},
        {0,1},
        {-1, 0},
        {0,0}
};
const int dirs = 5; /*amount of directions*/

/** The validateBounds function ensures that no possible move will result in the creature leaving the high bed
 * @param bounds - the array of potential moves */
void validateBounds(game* g, int bounds[]){
    int hbedSize = g->pr.i.paramStructInt.hbedsize;
    for (int i = 0; i < 4; i++){
        if (bounds[i] < 0){ bounds[i] = 0;}
        else if(bounds[i] >= hbedSize){ bounds[i] = hbedSize - 1; }
    }
}

/** The collectNearby function adds all creatures of a given species within range to an array
 * @param g - the game struct
 * @param p - the position of the current creature
 * @param bounds - the range
 * @param nearby - the array to collect the creatures in
 * @param target - the species being searched for
 * @return - the amount of creatures in the array
 */
int collectNearby(game* g, pos p, const int bounds[], pos nearby[], int target){
    int stackno = 0;
    for (int i = bounds[EAST]; i >= bounds[WEST]; i--) { /*collect nearby creatures*/
        for (int j = bounds[NORTH]; j <= bounds[SOUTH]; j++) {
            if ((p.column != j || p.row != i) && g->hbed[i][j].occupier.species == target) { /*if not the current creature && the species is the target*/
                pos x;
                x.row = i; x.column = j;
                nearby[stackno] = x;
                stackno++;
            }
        }
    }
    return stackno;
}

/** the inRange function checks whether a given species of creature is in range
 * @param g - the game struct
 * @param p - position of the current creature
 * @param range - the distance the current creature can see
 * @param target - the target species
 * @return - the position of a random nearby creature, (-1,-1) if none exist
 */
pos inRange(game* g, pos p, int range, species target){
    int bounds[] = {(p.column - range), (p.row + range), (p.column + range), (p.row - range)}; /*set bounds*/
    validateBounds(g, bounds);

    int grid = ((range * 2) + 1);
    pos nearby[(grid * grid) - 1]; /*array to hold results*/

    int stackno = collectNearby(g, p, bounds, nearby, target);
    if (stackno > 1) { /*return random result*/
        int r = rand() % (stackno - 1);
        return nearby[r];
    } else if (stackno == 1){ /*to avoid dividing by 0*/
        return nearby[0];
    }else{
        pos none;
        none.column = -1;
        none.row = -1;
        return none;
    }
}

/** the checkSpace function checks whether a given space is in bounds or occupied
 * @param g - the game struct
 * @param p - position of the current creature
 * @return - whether a space is valid or not */
bool checkSpace(game* g, pos p){
    bool safe = true;
    int hbedSize = g->pr.i.paramStructInt.hbedsize;
    if((p.row < 0 || p.column < 0)){ safe = false; } /*if below bound*/
    else if (p.row >= hbedSize || p.column >= hbedSize){ safe = false; } /*if above bound*/
    else { species sp = g->hbed[p.row][p.column].occupier.species; if (sp != BLANK){ safe = false; } } /*if occupied*/
    return safe;
}

/** the emptySpace function empties a given space
 * @param g - the game struct
 * @param p - position of the current creature */
void emptySpace(game* g, pos p){
        creature blank;
        blank.species = BLANK;
        g->hbed[p.row][p.column].occupier = blank;
}

/** the printMoveResult function outputs the result of a move
 * @param g - the game struct
 * @param p - the position of the creature which has moved
 * @param species - the species of the creature which moved */
void printMoveResult(game* g, pos p, int species){
    if (g->mode == '0') {
        printHBed(g);
        printPos(species,MOVE, p);
    }
}

/** the checkStuck function finds whether a creature can move
 * @param g - the game struct
 * @param p - the position of the creature which wants to move
 * @param speed - the amount the creature will move by
 * @return -  true if stuck */
bool checkStuck(game* g, pos p, int speed){
    if (g->hbed[p.row][p.column].occupier.species == FROG) {
        if (inRange(g, p, speed, BLANK).row != -1) { return false; }
    } else { for (int i = 0; i < dirs; i++) {
            pos dir;
            dir.row = p.row + (vector[i].row * speed);
            dir.column = p.column + (vector[i].column * speed);
            if (checkSpace(g, dir) && g->hbed[dir.row][dir.column].occupier.species == BLANK) { return false; }
        }
    } return true;
}

/** the crMove function moves a creature within the high bed
 * @param g - the game struct
 * @param p - the position of the creature which has moved
 * @param speed - the amount the creature will move by
 * @return -  true if motion is successful, else false */
bool crMove(game* g, pos p, int speed){
    creature cr = g->hbed[p.row][p.column].occupier;
    bool stuck = false;

    if(checkStuck(g, p, speed)){ /*if no spaces available*/
        stuck = true;
        if (g->mode == '0') {
            printHBed(g);
            printSpecies(cr.species, STUCK);
        }
    }

    if (speed != 0 && !stuck) { /*move*/
        emptySpace(g, p); /*empty old position*/

        pos new;
        new.column = -1; new.row = -1;
        while (!checkSpace(g, new)) { /*while not in bounds*/
            if (cr.species == FROG) { new = inRange(g, p, speed, BLANK); } /*frog jump to new position*/
            else if (cr.species == SLUG){
                new.row = p.row + (vector[cr.direction].row * speed);
                new.column = p.column + (vector[cr.direction].column * speed); /*slug crawl to new position*/
                if (!checkSpace(g,new)) { cr.direction = rand() % 4;} /*change direction, if out of bounds*/
            }
        }

        g->hbed[new.row][new.column].occupier = cr; /*add to new position*/
        if (g->mode == '0') { printMoveResult(g, new, cr.species); }
        return true;
    } else{ return false; }
}





