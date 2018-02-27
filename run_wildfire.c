///
/// File: run_wildfire.c
///
/// @author kjb2503 : Kevin Becker
///
// // // // // // // // // // // // // // // // // // // // // // //

#define _BSD_SOURCE
#include <unistd.h> // usleep
#include <stdio.h> // printing
#include <stdlib.h> // rand
#include "display.h" // cursor-controlled output functions
#include "wildfire_values.h" // some default constants

// NEIGHBOR_DNE is used in shouldChange
#define NEIGHBOR_DNE -1


///
/// Function: getPrintCharacter
///
/// Description: Returns the character which should be printed.
///
/// @param status  The current status of the cell on the board.
///
/// @return The character which should be printed.
///
static char getPrintChar(int value)
{
    // 0 is space
    // 1 is alive tree
    // 3 is burnt tree
    // everything else is burning
    switch(value)
    {
        case EMPTY_VALUE:
            return EMPTY;
            break;
        case ALIVE_VALUE:
            return ALIVE;
            break;
        case BURNT_VALUE:
            return BURNT;
            break;
        default:
            return BURNING;
    }
}


///
/// Function: shouldChange
///
/// Description: Determines if a cell is allowed to ``change'' to a different
///              status. (i.e. ALIVE to BURNING, BURNING to BURNT, etc.).
///
/// @param row  The row of the cell we are checking.
/// @param col  The column of the cell we are checking.
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
/// @return an integer; 0 if should NOT change, 1 otherwise.
///
static int shouldChange(int row, int col, int size, char simBoard[][size])
{
    // returns 1 if the tree is already burning
    // (the next logic only affects non-burning trees)
    if(simBoard[row][col] == BURNING_VALUE)
        return 1;

    // the total neighbors
    int totalNeighbors = 0;
    // the total neighbors which are BURNING
    float totalBurning = 0.0;
    
    // if we get here we are alive, and need to check neighbors
    // can't have NE neighbor if at row 0 OR col 0
    // we pull in all of our spaces before we do anything else, determine what
    // to do after we get this information
    int NW = (row > 0 && col > 0) ? simBoard[row-1][col-1] : NEIGHBOR_DNE;
    int N = (row > 0) ? simBoard[row-1][col] : NEIGHBOR_DNE;
    int NE = (row > 0 && col < size-1) ? simBoard[row-1][col+1] : NEIGHBOR_DNE;
    int E = (col < size-1) ? simBoard[row][col+1] : NEIGHBOR_DNE;
    int SE = (row < size-1 && col < size-1) ? simBoard[row+1][col+1] : NEIGHBOR_DNE;
    int S = (row < size-1) ? simBoard[row+1][col] : NEIGHBOR_DNE;
    int SW = (row < size-1 && col > 0) ? simBoard[row+1][col-1] : NEIGHBOR_DNE;
    int W = (col > 0) ? simBoard[row][col-1] : NEIGHBOR_DNE;
    
    // performs a check on each possible neighbor adding to items as needed
    if(NW != NEIGHBOR_DNE)
    {
        // adds to total neighbors if cell contains a tree (in any form)
        totalNeighbors += (NW == ALIVE_VALUE || NW == BURNING_VALUE) ? 1 : 0;
        // adds to total burning if cell contains a burning tree
        totalBurning += (NW == BURNING_VALUE) ? 1 : 0;
    }
    if(N != NEIGHBOR_DNE)
    {
        totalNeighbors += (N == ALIVE_VALUE || N == BURNING_VALUE) ? 1 : 0;
        totalBurning += (N == BURNING_VALUE) ? 1 : 0;
    }
    if(NE != NEIGHBOR_DNE)
    {
        totalNeighbors += (NE == ALIVE_VALUE || NE == BURNING_VALUE) ? 1 : 0;
        totalBurning += (NE == BURNING_VALUE) ? 1 : 0;
    }
    if(E != NEIGHBOR_DNE)
    {
        totalNeighbors += (E == ALIVE_VALUE || E == BURNING_VALUE) ? 1 : 0;
        totalBurning += (E == BURNING_VALUE) ? 1 : 0;
    }
    if(SE != NEIGHBOR_DNE)
    {
        totalNeighbors += (SE == ALIVE_VALUE || SE == BURNING_VALUE) ? 1 : 0;
        totalBurning += (SE == BURNING_VALUE) ? 1 : 0;
    }
    if(S != NEIGHBOR_DNE)
    {
        totalNeighbors += (S == ALIVE_VALUE || S == BURNING_VALUE) ? 1 : 0;
        totalBurning += (S == BURNING_VALUE) ? 1 : 0;
    }
    if(SW != NEIGHBOR_DNE)
    {
        totalNeighbors += (SW == ALIVE_VALUE || SW == BURNING_VALUE) ? 1 : 0;
        totalBurning += (SW == BURNING_VALUE) ? 1 : 0;
    }
    if(W != NEIGHBOR_DNE)
    {
        totalNeighbors += (W == ALIVE_VALUE || W == BURNING_VALUE) ? 1 : 0;
        totalBurning += (W == BURNING_VALUE) ? 1 : 0;
    }
    
    // if we have more than 25% neighbors on fire, we return 1, otherwise 0
    return ((totalBurning / totalNeighbors) > 0.25) ? 1 : 0;
}


///
/// Function: update
///
/// Description: Updates the wildfire simulation board for the next cycle.
///
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
/// @return the number of changes this iteration
///
static int update(int probability, int size, char simBoard[][size])
{
    int row, col, numberOfChanges = 0;
    
    char nextCycle [size][size];
    
    for(row = 0; row < size; ++row)
        for(col = 0; col < size; ++col)
            switch(simBoard[row][col])
            {
                // if space is empty, we don't care
                case EMPTY_VALUE:
                case BURNT_VALUE:
                    nextCycle[row][col] = simBoard[row][col];
                    break;
                // if we are alive or burning, we need to do some logic (shares
                // a lot of the same conditions so we only use single case)
                default:
                    // if we should change and probability is met
                    if(shouldChange(row, col, size, simBoard) && 
                       (rand()%100) < probability)
                    {
                        // set the tree on fire/extinguish
                        nextCycle[row][col] = (simBoard[row][col] == ALIVE_VALUE) ? 
                                                BURNING_VALUE : BURNT_VALUE;
                        // record the change
                        ++numberOfChanges;
                    }
                    // otherwise we stay the same
                    else
                        nextCycle[row][col] = simBoard[row][col];
            }
    
    // copies over new cycle to simBoard
    for(row = 0; row < size; ++row)
        for(col = 0; col < size; ++col)
            simBoard[row][col] = nextCycle[row][col];
    // returns the number of changes
    return numberOfChanges;
}

///
/// Function: numberOfBurningTrees
///
/// Description: Returns the number of burning trees (used to stop simulation if
///              returned value is 0).
///
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
static int numberOfBurningTrees(int size, char simBoard[][size])
{
    int burningTrees = 0;
    // goes through all rows and cols determining the number of alive fires
    for(int row = 0; row < size; ++row)
    {
        for(int col = 0; col < size; ++col)
        {
            // if our fire is still alive, add one to the burning variable
            if(simBoard[row][col] == BURNING_VALUE)
                ++burningTrees;
        }
    }
    
    // return the number of burning trees
    return burningTrees;
}


static void printFiresOut(int numberOfChanges)
{
    // prints that we have run out of fires
    printf("fires are out after %d cumulative changes\n", numberOfChanges);
}


static void printBoard(const char *statusFormatString, int cycle, 
                       int numberOfChanges, int size, char simBoard[][size])
{
    // prints our board
    for(int row = 0; row < size; ++row)
    {
        for(int col = 0; col < size; ++col)
            printf("%c", getPrintChar(simBoard[row][col]));
        puts(" ");
    }

    // prints our status
    printf(statusFormatString, cycle, numberOfChanges);
}


///
/// Function: runIterations
///
/// Description: Runs a specified number of iterations of wildfire printing out
///              each iteration.
///
/// @param *statusFormatString  A format string used to print out status.
/// @param numberOfIterations  The number of iterations to run (-1 = indeterminate).
/// @param probability  The probability of tree catching fire/extinguishing.
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
static void runIterations(const char *statusFormatString, int numberOfIterations,
                          int probability, int size, char simBoard[][size])
{
    // a few variables we
    int numberOfChanges = 0, cycle = 0;
    
    // we want to print out a cycle 0 therefore do{}while(...)
    do {
        // prints our board
        printBoard(statusFormatString, cycle, numberOfChanges, size, simBoard);
        
        // updates and adds the number of changes
        numberOfChanges += update(probability, size, simBoard);
    } while(++cycle <= numberOfIterations && 
            numberOfBurningTrees(size, simBoard));  
    // the above while goes until all trees are burnt out, or we hit our number
    // of iterations
    
    // if no trees burning still, print message telling that all fires are out
    if(!numberOfBurningTrees(size, simBoard))
    {
        // prints our board
        printBoard(statusFormatString, cycle, numberOfChanges, size, simBoard);
        // prints that our fires are out
        printFiresOut(numberOfChanges);
    }
    
}


///
/// Function: updateDisplayedBoard
///
/// Description: Updates the displayed board on the screen.
///
/// @param *statusFormatString  A format string used to print out status.
/// @param cycle  The current cycle number.
/// @param numberOfChanges  The total number of changes in the simulation.
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
static void updateDisplayedBoard(const char *statusFormatString, int cycle,
                       int numberOfChanges, int size, char simBoard[][size])
{
    for(int row = 0; row < size; ++row)
    {
        // sets our cursor
        set_cur_pos(row+1,0);
        // prints each char in the column
        for(int col = 0; col < size; ++col)
            put(getPrintChar(simBoard[row][col]));
    }
    
    // sets our current position to size+1 (after last row)
    set_cur_pos(size+1, 0);
    
    // prints our status
    printf(statusFormatString, cycle, numberOfChanges);
}


///
/// Function: runIndeterminate
///
/// Description: The indeterminate version of the wildfire simulation. Uses
///              cursor controlled output to overlay each cycle of the simulation.
///
/// @param *statusFormatString  A format string used to print out status.
/// @param probability  The probability of tree catching fire/extinguishing.
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
static void runIndeterminate(const char *statusFormatString, int probability, 
                             int size, char simBoard[][size])
{
    // clears the screen and set cursor to top left before we do anything else
    clear();
    
    // sets our two variables we need to keep track of
    int numberOfChanges = 0, cycle = 0;
    
    // keeps going until our trees are out, or user hits CTRL-C
    // if loop exits, numberOfBurningTrees returns 0;
    while(numberOfBurningTrees(size, simBoard))
    {
        // updates our displayed board
        updateDisplayedBoard(statusFormatString, cycle, numberOfChanges, size, simBoard);
        
        set_cur_pos(size+2, 0);
        printf("4\n");
        // sleep for 4s
        usleep(1000000);
        set_cur_pos(size+2, 0);
        printf("3\n");
        usleep(1000000);
        set_cur_pos(size+2, 0);
        printf("2\n");
        usleep(1000000);
        set_cur_pos(size+2, 0);
        printf("1\n");
        usleep(1000000);
        
        // updates our board adding the return value to numberOfChanges
        numberOfChanges += update(probability, size, simBoard);
        
        // lastly we need to add to our cycle number
        ++cycle;
    }
    
    // prints our board one last time
    updateDisplayedBoard(statusFormatString, cycle, numberOfChanges, size, simBoard);
    
    printFiresOut(numberOfChanges);
    
    // once we exit the loop, we can print that we have completed
    printf("fires are out after %d cumulative changes\n", numberOfChanges);
}


///
/// Function: runWildfire
///
/// Description: Runs a simulation of wildfire
///
/// @param *statusFormatString  A format string used to print out status.
/// @param numberOfIterations  The number of iterations to run (-1 = indeterminate).
/// @param probability  The probability of tree catching fire/extinguishing.
/// @param size  The size of the board.
/// @param simBoard  The simulation board.
///
void runWildfire(const char *statusFormatString, int numberOfIterations, 
                 int probability, int size, char simBoard[][size])
{
    // determines if user wanted a specific number of iterations
    if(numberOfIterations != -1)
        runIterations(statusFormatString, numberOfIterations, probability, 
                      size, simBoard);
    else
        runIndeterminate(statusFormatString, probability, size, simBoard);
}

