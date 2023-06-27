/*
Created by Cassidy on 06/12/2022.
*/
#include "main.h"
#include <stdio.h>

/** the printEat function outputs when a creature eats
 * @param g - the Game struct
 * @param cr - the hungry creature's species
 * @param food - the position of the food */
void printEat(game* g, int species, pos food){
    printHBed(g);
    printf("Oooh, a meal!\n");
    printPos(species, MOVE, food);
}

/** the eat function lets a creature eat another creature
 * @param g - the Game struct
 * @param hungry - the position of the hungry creature
 * @param food - the position of the food */
void eat(game* g, pos hungry, pos food){
    creature cr = g->hbed[hungry.row][hungry.column].occupier; /*save hungry*/
    emptySpace(g,hungry); /*empty old hungry position*/

    cr.hunger = 0;
    g->hbed[food.row][food.column].occupier = cr; /*eat*/
}