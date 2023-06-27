#include <malloc.h>
#include <time.h>
#include "main.h"
#include "load.h"
#include "breed.h"
#include "eat.h"

/** The eol function checks whether a creature has reached the end of its lifespan
 * @param g - the game struct
 * @param p  - the position of the creature to check
 * @return - true, if creature has reached the end of its lifespan */
bool eol(game* g, pos p){ //return whether a creature vanishes
    creature cr = g->hbed[p.row][p.column].occupier;
    if ( //if lifespan is over, vanish
            (cr.species == SLUG && cr.age > g->pr.i.paramStructInt.lifespanSlug) ||
            (cr.species == FROG && (cr.age > g->pr.i.paramStructInt.lifespanFrog))
            ) {
        emptySpace(g,p); //empty creature's square

        if (g->mode == '0') {
            printHBed(g);
            printSpecies(cr.species, AWAY);
        }
        return true;
    }
    g->hbed[p.row][p.column].occupier.age += 1; //age creature
    return false;
}

/** The tryEat function checks whether an edible creature is in range, then eats it if there is.
 * @param g - the game struct
 * @param p  - the position of the creature to check
 * @param cr - the species of the hungry creature
 * @return - whether a creature was eaten */
bool tryEat(game* g, pos p, creature cr){
    pos food;
    food = inRange(g, p, g->pr.i.paramArray_int[(cr.species == SLUG) ? visionSLUG : visionFROG],(cr.species == SLUG) ? LETTUCE : SLUG); //lettuce, if slug - else slug

    if (food.row > -1) { //if food in range, eat it
        eat(g, p, food);
        if (g->mode == '0') { printEat(g, cr.species, food); }
        return true;
    }
    if (cr.species == FROG) { g->hbed[p.row][p.column].occupier.hunger += 1; } //increase hunger if frog not eating
    return false;
}

/** The tryEat function checks whether a mate is in range, then attempts to spawn if there is.
 * @param g - the game struct
 * @param p  - the position of the creature to check
 * @param cr - the species of the first parent
 * @return - whether a child creature was created */
bool tryBreed(game* g, pos p, creature cr){
    pos mate; //else try to spawn
    mate = inRange(g, p, g->pr.i.paramArray_int[(cr.species == SLUG) ? visionSLUG : visionFROG], (cr.species == SLUG) ? SLUG : FROG); //check for mates

    if (mate.row != -1 && breedCheck(g, p, mate)) { spawn(g, p); return true; } //if mature mate in range, attempt to spawn
    else {return false;}
}

/** The tryMove function checks whether creature can move, then moves if it can.
 * @param g - the game struct
 * @param p  - the position of the creature to check
 * @return - whether the creature moved */
bool tryMove(game* g, pos p){
    int speed;
    int species = g->hbed[p.row][p.column].occupier.species;
    if (species == FROG && g->hbed[p.row][p.column].occupier.hunger >= g->pr.i.paramStructInt.hungryFrog) { //if frog is hungry
        speed = g->pr.i.paramStructInt.visionFrog;
        crMove(g, p, speed);
        return true;
    } else if (species == SLUG) {
        speed = g->pr.i.paramStructInt.visionSlug;
        crMove(g, p, speed);
        return true;
    } else{
        if (g->mode == '0') {
            printHBed(g);
            printSpecies(species, STOOD);
        }
    }
    return false;
}

/** the act function decides the action a creature should take in a given turn.
 * @param g - the game struct
 * @param p  - the position of the creature to act */
void act(game* g, pos p) {
    creature cr = g->hbed[p.row][p.column].occupier;
    switch (cr.species) {
        case LETTUCE: { //duplicate randomly based on lettucegrowprob
            if (breedCheck(g, p, p)) {
                spawn(g, p);
            } else if (g->mode == '0') {
                printHBed(g);
                printf("A Lettuce stood still\n");
            }
            break;
        }
        case SLUG: //fallthrough
        case FROG: { //first try to eat, else try to spawn, else try to move
            if (!tryEat(g, p, cr)) { if (!tryBreed(g, p, cr)) { tryMove(g, p); } }
            break;
        }
        default: { break; }
    }
}

/** the getMode function lets the user choose the output mode
 * @param g - the game struct */
void getMode(game* g){
    clearScreen();
    do { //get mode
        printf("Do you want to play:\n0 - Moves mode\n1 - Rounds mode\n");
        fflush(stdin);
        g->mode = getchar();
    } while (g->mode != '1' && g->mode != '0');
}

/** The addStack function adds all living creatures to a stack
 * @param g - the game struct
 * @param stack - the stack array
 * @param stackmult - the multiplier for growing the stack
 * @return - the amount of creatures in the stack */
int addStack(game* g, pos** stack, int* stackmult){
    int hbedSize = g->pr.i.paramStructInt.hbedsize;
    int stackno = 0;
    for (int i = 0; i < hbedSize; i++) { //find creatures row
        for (int j = 0; j < hbedSize; j++) { //column
            if (stackno >= (hbedSize * *stackmult - 1)){ //expand array if full
                *stackmult += 1;
                *stack = realloc(*stack, (hbedSize * sizeof(pos)) * *stackmult);
#ifdef STACKDEBUG
                printf("\n");
                    for (i = 0; i < stackno; ++i) {
                        printf("%i,%i ",stack[0][i].row,stack[0][i].column);
                    }
                    fflush(stdin);
                    getchar();
#endif
            }
            if (g->hbed[i][j].occupier.species != BLANK) { //if creature is not blank, add to array of living creatures
                stack[0][stackno].row = i;
                stack[0][stackno].column = j;
                stackno++;
            }
        }
    } return stackno;
}

/** the checkContinue function asks the user whether to continue the simulation
 * @param choice - the address to write the user's response to */
void checkContinue(char* choice){
    do {
        printf("Press <enter> to continue, or q to quit!");
        fflush(stdin);
        *choice = getchar();
#ifdef DEBUG
        *choice = '\n';
#endif
    } while(*choice != '\n' && *choice != 'q');
}

/** the checkEndgame function checks whether a game is over
 * @param g - the game struct
 * @param choice - the address to write whether the game is over to
 * @param stack - an array of living creatures
 * @param stackmult - the multiplier for growing the stack */
void checkEndgame(game* g, char* choice, pos** stack, int* stackmult){
    int stackno = addStack(g, stack, stackmult); //refresh stack

    *choice = 'q';
    for (int i = 1; i < stackno; i++){ //check whether all but one species is extinct
        if (g->hbed[stack[0][i].row][stack[0][i].column].occupier.species != g->hbed[stack[0][i-1].row][stack[0][i-1].column].occupier.species){ //if current species != previous species
            *choice = '0';
        }
    }
    if (*choice == 'q'){ //if all living creatures are of one species
        printHBed(g);
        printSpecies(g->hbed[stack[0][0].row][stack[0][0].column].occupier.species, END);
        fflush(stdin);
        getchar();
    }
}

/** the runRounds function loops game rounds until the user quits or endgame is reached
 * @param g - the game struct */
void runRounds(game* g){
    char choice = '0'; //initialise game variables
    int round = 0;
    int stackmult = 1;
    pos* stack = (pos*)malloc(( g->pr.i.paramStructInt.hbedsize * sizeof(pos)) * stackmult);

    printHBed(g);
    printf("The simulation is ready to begin!\n");
    checkContinue(&choice);
    checkEndgame(g, &choice, &stack, &stackmult);
    while (choice != 'q') { //while not quit or finished
        pos p;

        int stackno = addStack(g, &stack, &stackmult);
        int i = 0;
        while (i < (stackno) && choice != 'q'){ //for all creatures in the stack, while game has not ended
            p.row = stack[i].row;
            p.column = stack[i].column;

            if (!eol(g,p)){ act(g, p); } //if not vanishing, act
            if (g->mode == '0') { //in moves mode
                checkEndgame(g, &choice, &stack, &stackmult);
                if (choice != 'q'){ checkContinue(&choice); }
            }
            i++;
        }
        if (g->mode == '1') { //in rounds mode
            round++;
            printHBed(g);
            checkEndgame(g, &choice, &stack, &stackmult);
            printf("Round %i complete.\n", round);
            if (choice != 'q'){ checkContinue(&choice); }
        }
    }
}

/** the initGame function loads data from the config, if present, and initialises additional parameters or variables
 * @param g - the game struct */
void initGame(game* g){
    data* config = malloc(sizeof(data)); //initialise config struct
    getFromFile(config); //load data

    g->pr.f = config->pr_float; //assign parameters
    g->pr.i = config->pr_int;

    int hbedsize = g->pr.i.paramStructInt.hbedsize; //resize game based on hbedSize
    g->hbed = malloc((sizeof(g->pr) + sizeof(g->mode) + (hbedsize * sizeof(cell*)) ));
    for (int i = 0; i < hbedsize; i++) { g->hbed[i] = (cell *) malloc(hbedsize * sizeof(cell)); }

    addToHbed(g, config->creatures); //assign creatures
    free(config);

    g->pr.i.paramStructInt.visionSlug = SLUG_VISION; //add additional parameters

    g->mode = -1; //initialise variables
}

/** set the random number seed to produce unpredictable random sequences */
void set_random_seed(){
    time_t t = time(0); // time is constantly changing and makes a good seed
    srand(t);
}

/** The main function starts the simulation */
int main() {
    game* g =  malloc(sizeof(game));

    set_random_seed();
    initGame(g);
    getMode(g);
    runRounds(g);
}
