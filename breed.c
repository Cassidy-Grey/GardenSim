/*
  Created by Cassidy on 06/12/2022.
*/
#include "breed.h"
#include "stdio.h"
#include "stdlib.h"

/** the breedCheck function return whether two creatures are able to spawn
 * @param g - the game struct
 * @param p - the position of the first parent
 * @param q - the position of the second parent
 * @return - whether breeding is possible */
bool breedCheck(game* g, pos p, pos q) {
    float x = (rand() % 100);
    x = x / 100;
    int age = g->hbed[p.row][p.column].occupier.age;
    int age2 = g->hbed[q.row][q.column].occupier.age;
    switch(g->hbed[p.row][p.column].occupier.species) {
        case LETTUCE: { if (x < g->pr.f.paramStructFloat.growLettuce){ return true; } break; } /*return true or false at random, based on lettuce growth probability*/
        case SLUG:
            if ( age < g->pr.i.paramStructInt.matureSlug || age2 < g->pr.i.paramStructInt.matureSlug) { return false;} /*if either slug is immature*/
            else if (x < g->pr.f.paramStructFloat.reproduceSlug){ return true; }
            break;
        case FROG:
            if (age < g->pr.i.paramStructInt.matureFrog || age2 < g->pr.i.paramStructInt.matureFrog) { return false; } /*if either frog is immature*/
            else if (x < g->pr.f.paramStructFloat.reproduceFrog){ return true; }
            break;
        default: break;
    } return false;
}


/** the spawn function spawns a child based on the position of its first parent
 * @param g - the Game struct
 * @param hungry - the position of the first parent */
void spawn(game* g, pos parent){
    pos spawn = inRange(g, parent, 1, BLANK);

    if(spawn.row != -1) {
        creature child; //assign child
        child.species = g->hbed[parent.row][parent.column].occupier.species;
        child.direction = (child.species == SLUG) ? (rand() % 4) : STILL;
        child.age = 0;
        child.hunger = 0;

        g->hbed[spawn.row][spawn.column].occupier = child;

        if (g->mode == '0'){
            printHBed(g);
            printPos(child.species, APPEAR, spawn);
        }
    } else{
        if (g->mode == '0') {
            printHBed(g);
            printSpecies(g->hbed[parent.row][parent.column].occupier.species, SPAWNFAIL);
            printf("But there was no room...\n");
        }
    }
}
