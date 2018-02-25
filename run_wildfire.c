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
#include <unistd.h> // usleep
#include <stdio.h> // printing
#include <stdlib.h> // rand
#include "display.h" // cursor-controlled output functions
#include "wildfire_values.h" // some default constants


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
    // -1 is burnt tree
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


static int shouldChange(int row, int col, int size, char simBoard[][size])
{
    // returns ``true'' if the tree is already burning
    // (this only affects non-burning trees)
    if(simBoard[row][col] == BURNING_VALUE)
        return 1;
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
        for(col = 0; col < size; ++col)
            switch(simBoard[row][col])
            {
                // if space is empty, we don't care
                case EMPTY_VALUE:
                case BURNT_VALUE:
                    nextCycle[row][col] = simBoard[row][col];
                    break;
                // if we are alive or burning, we need to do some logic
                default:
                    if(canChange(row, col, size, simBoard)
                        && (rand()%100) < probability)
                    {
                        // set the tree on fire/extinguish if prob good
                        nextCycle[row][col] = (simBoard[row][col] == ALIVE_VALUE) ? 
                                                IGNITE_VALUE : BURNT_VALUE;
                        // record the change
                        ++numberOfChanges;
                    }
                    else
                        nextCycle[row][col] = simBoard[row][col];
            }
    
    // copies over new cycle
    for(row = 0; row < size; ++row)
        for(col = 0; col < size; ++col)
            simBoard[row][col] = nextCycle[row][col];
    
    return numberOfChanges;
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
                printf("%c", getPrintChar(simBoard[row][col]));
            puts(" ");
        }
        
        // prints our status
        printf(statusFormatString, cycle, numberOfChanges);
        
        // updates and adds the number of changes
        numberOfChanges += update(size, simBoard);
    }
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
            if(simBoard[row][col] == IGNITE_VALUE)
                ++burningTrees;
        }
    }
    
    // return the number of burning trees
    return burningTrees;
}

static void printBoard(const char *statusFormatString, int cycle,
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
    
    set_cur_pos(size, 0);
    
    // prints our status
    printf(statusFormatString, cycle, numberOfChanges);
}


static void runIndeterminate(const char *statusFormatString, int size, char simBoard[][size])
{
    // clears the screen and set cursor to top left before we do anything else
    clear();
    
    // sets our two variables we need to keep track of
    int numberOfChanges = 0, cycle = 0;
    
    // keeps going until our trees are out, or user hits CTRL-C
    while(numberOfBurningTrees(size, simBoard) != 0)
    {
        printBoard(statusFormatString, cycle, numberOfChanges, size, simBoard);
        
        // sleep for 250 ms
        usleep(750000);
        
        // updates our board
        numberOfChanges += update(size, simBoard);
        
        // lastly we need to add to our cycle number
        ++cycle;
    }
    
    // prints our board one last time
    printBoard(statusFormatString, cycle, numberOfChanges, size, simBoard);
    // once we exit the loop, we can print that we have completed
    printf("fires are out after %d cumulative changes\n", numberOfChanges);
}


void runWildfire(const char *statusFormatString, int size, char simBoard[][size])
{
    if(numberOfIterations != -1)
        runIterations(statusFormatString, size, simBoard);
    else
        runIndeterminate(statusFormatString, size, simBoard);
}


