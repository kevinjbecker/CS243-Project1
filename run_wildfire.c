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


// pulls in our information from the other file
int probability, numberOfIterations;


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
    // 12 is burnt tree
    // everything else is burning
    switch(value)
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


///
///
///
///
///
/// @return the number of changes this iteration
///
static int update(int size, char simBoard[][size])
{
    int row, col, numberOfChanges = 0;
    
    char nextCycle [size][size];
    
    for(row = 0; row < size; ++row)
    {
        for(col = 0; col < size; ++col)
        {
            nextCycle[row][col] = 0;
        }
    }
    
    // copies over new cycle
    for(row = 0; row < size; ++row)
        for(col = 0; col < size; ++col)
            simBoard[row][col] = nextCycle[row][col];
    
    return numberOfChanges;
}


static int numberOfBurningTrees(int size, char simBoard[][size])
{
    int burningTrees = 0;
    // goes through all rows and cols determining the number of alive fires
    for(int row = 0; row < size; ++row)
    {
        for(int col = 0; col < size; ++col)
        {
            // if our fire is still alive, add one to the burning variable
            if(simBoard[row][col] > IGNITE_VALUE && 
               simBoard[row][col] < EXTINGUISH_VALUE)
                ++burningTrees;
        }
    }
    
    // return the number of burning trees
    return burningTrees;
}


static void runIterations(const char *statusFormatString, int size, char simBoard[][size])
{
    // a few variables we
    int numberOfChanges = 0;
   
    // goes through from 0 to the number of iterations requested
    for(int cycle = 0; cycle <= numberOfIterations; ++cycle)
    {
        // prints our board
        for(int row = 0; row < size; ++row)
        {
            for(int col = 0; col < size; ++col)
                printf("%c ", getPrintChar(simBoard[row][col]));
            puts(" ");
        }
        
        // prints our status format string
        printf(statusFormatString, cycle, numberOfChanges);
        
        // updates and adds the number of changes
        numberOfChanges += update(size, simBoard);
    }
}


static void runIndeterminate(const char *statusFormatString, int size, char simBoard[][size])
{
    // clears the screen before we do anything else
    clear();
    // sets our two variables we need to keep track of
    int numberOfChanges = 0, cycle = 0;
    while(numberOfBurningTrees(size, simBoard) > 0)
    {
        ++cycle;
        numberOfChanges += update(size, simBoard);
        printf(statusFormatString, cycle, numberOfChanges);
        // sleep for 250 ms
        usleep(250000);
    }
    // once we exit the loop, we can print that we have completed
    printf("fires are out after %d cumulative changes", numberOfChanges);
}


void runWildfire(const char *statusFormatString, int size, char simBoard[][size])
{
    if(numberOfIterations != -1)
        runIterations(statusFormatString, size, simBoard);
    else
        runIndeterminate(statusFormatString, size, simBoard);
}


