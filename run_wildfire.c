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

void runIterations(int numberOfIterations, int size, char simBoard[][size])
{
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
    while(numberOfBurningTrees > 0)
    {
        
    }
}
