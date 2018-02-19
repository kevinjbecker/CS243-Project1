///
///
///
///
///
///
///
///
///
// // // // // // // // // // // // // // // // // // // // // // //

#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include "display.h"
#include "wildfire_values.h"


///
///
///
///
///
/// @return the number of changes this iteration
///
static int update(int probability, int size, char simBoard[][size])
{
    
}


///
/// Function: getPrintCharacter
///
/// Description: Returns the character which should be printed.
///
/// @param status  The current status of the cell on the board.
///
/// @return The character which should be printed.
///
static char getPrintCharacter(int status)
{
    // 0 is space
    // 1 is alive tree
    // 12 is burnt tree
    // everything else is burning
    switch(status)
    {
        case 0:
            return EMPTY;
            break;
        case 1:
            return ALIVE;
            break;
        case 12:
            return BURNT;
            break;
        default:
            return BURNING;
    }
}


void runIterations(int numberOfIterations, int size, char simBoard[][size])
{
    int numberOfChanges = 0;
    for(int i = 0; i < numberOfIterations; ++i)
    {
        
    }
}


static int numberOfBurningTrees(int size, char simBoard[][size])
{
    int burningTrees = 0;
    
    for(int row = 0; row < size; ++row)
        for(int col = 0; col < size; ++col)
            if(simBoard[row][col] > IGNITE_VALUE && simBoard[row][col] < EXTINGUISH_VALUE)
                ++burningTrees;
    // return the number of burning trees
    return burningTrees;
}

void runIndeterminate(int size, char simBoard[][size])
{
    // clears the screen before we do anything else
    clear();
    // sets our two variables we need to keep track of
    int numberOfChanges = 0, cycles = 0;
    while(numberOfBurningTrees > 0)
    {
        // sleep for 81000 ms
        usleep(81000);
    }
}
