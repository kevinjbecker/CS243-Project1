///
///
///
///
///
///
///
///
///
/// @author: kjb2503 : Kevin Becker
///
// // // // // // // // // // // // // // // // // // // // // // // // // // //

#include <stdio.h> // fprintf
#include <stdlib.h> // EXIT_SUCCESS
#include <time.h> //time
#include "run_wildfire.h" // runIterataions, runIndeterminate
#include "wildfire_values.h" // all of the shared values

///
/// Function: printUsageMsg
///
/// Description: Displays the usage of the program.
///
static void printUsageMsg(char *cmdUsed)
{
    // prints our usage to stderr
    fprintf(stderr, "Usage: %s [-pN] size probability treeDensity proportionBurning\
    \nThe -pN option tells the simulation to print N cycles and stop.\
    \nThe probability is the probability a tree will catch fire.\n", cmdUsed);
}


///
/// Function: argumentError
///
/// Description: 
///
/// @param name
/// @param value
///
static void argumentError(char *cmdUsed, char *name, int value, char *constraints)
{
    fprintf(stderr, "The %s (%d) must be an integer %s.\n", 
        name, value, constraints);
    printUsageMsg(cmdUsed);
}


///
/// Function: generateSimBoard
///
/// Description: Generates a starting life board with the number of organisms 
///              requested by the user.
///
/// @param numOrgs  The number of ``organisms'' to put in our board.
/// @param size  The size of each row in the ``board''.
/// @param board  The life board.
///
static void initializeSimBoard(int density, 
                        int proportionBurning, 
                        int size, 
                        char simBoard[][size])
{
    // row, column, and index variables
    int row, col;
    // sets our random number generator to current time
    srand(time(NULL));

    // fills our board with 0s
    for(row = 0; row < size; ++row)
        for(col = 0; col < size; ++col)
            simBoard[row][col] = EMPTY_VALUE;

    // determines the total number of trees
    int totalTrees = (size * size) * (density / 100.0);
    // determines the number of burning trees
    int totalBurningTrees = totalTrees * (proportionBurning / 100.0);

    // dispurses our trees
    while(totalTrees > 0)
    {
        // determines a random location
        row = rand() % size;
        col = rand() % size;
        
        // if our space is already filled with something
        if(simBoard[row][col] == 0)
        {
            simBoard[row][col] = ALIVE_VALUE;
            --totalTrees;
        }
    }
    
    // lights some trees on fire
    while(totalBurningTrees > 0)
    {
        // determines a random location
        row = rand() % size;
        col = rand() % size;
        
        // if our space is already filled with something
        if(simBoard[row][col] == 1)
        {
            simBoard[row][col] = IGNITE_VALUE;   
            --totalBurningTrees;
        }
    }
}


///
/// Function: printBoard
///
/// Description: Prints a 2d represented board of our forest.
///
/// @param size  The square size of the board
/// @param simboard  The simBoard.
///
static void printBoard(int size, char simBoard[][size])
{
    for(int row = 0; row < size; ++row)
    {
        // print our characters
        for(int col = 0; col < size; ++col)
            printf("%c", getPrintCharacter(simBoard[row][col]));
        // prints our new line
        puts(" ");
    }
}


/// 
/// Function: main
///
/// Description: The main driver for the wildfire program.
///
/// @param argc  The number of arguments which the program was run with.
/// @param argv  A list of character arrays contain each argument.
///
/// @return an integer based on the status of the exit (0 is success).
///
int main(int argc, char **argv)
{
    // checks to make sure we have the right number of command line arguments 
    // (we need to abort otherwise)
    if(argc < 5 || argc > 6)
    {
        // prints our usage
        printUsageMsg(argv[0]);
        // exits
        return EXIT_FAILURE;
    }
    

    // the size of our board
    int size, treeDensity, proportionBurning, probability;
    // we might not need this... hmm
    // we set it to -1 (changed if we do not want to go forever)
    int numberOfIterations = -1;

    // offset is used for later on if we need an offset
    int offset = 0;

    // parses our command line inputs to get ready
    // if we get argc of 6, -pN was included
    if(argc == 6)
    {
		// checking for -p, if it's not we had a bad argument
		if (argv[1][0] != '-' || argv[1][1] != 'p')
		{
			printUsageMsg(argv[0]);
			return EXIT_FAILURE;
		}
        // start +2 to get past "-p" part
        numberOfIterations = strtol(argv[1]+2, NULL, 10);
        // set our offset (we need to boost everything up by one)
        offset = 1;
    }

    // reads in our size, prob, density and proportion values to variables
    size = strtol(argv[1 + offset], NULL, 10);
    probability = strtol(argv[2 + offset], NULL, 10);
    treeDensity = strtol(argv[3 + offset], NULL, 10);
    proportionBurning = strtol(argv[4 + offset], NULL, 10);

    // goes through and checks each command line argument for correct formatting
    if(numberOfIterations < 0 && offset == 1)
    {
        argumentError(argv[0], "number of iterations", numberOfIterations, 
            "greater than 0");
        return EXIT_FAILURE;
    }
    if(size < 5|| size > 40)
    {
        argumentError(argv[0], "size", size, "within [5-40]");
        return EXIT_FAILURE;
    }
    if (probability < 0 || probability > 100)
    {
        argumentError(argv[0], "probability", probability, "within [0-100]");
        return EXIT_FAILURE;
    }
    if (treeDensity < 0 || treeDensity > 100)
    {
        argumentError(argv[0], "tree density", treeDensity, "within [0-100]");
        return EXIT_FAILURE;
    }
    if (proportionBurning < 0 || proportionBurning > 100)
    {
        argumentError(argv[0], "proportion", proportionBurning, "within [0-100]");
        return EXIT_FAILURE;
    }
    
    // BOARD GENERATION SEQUENCE ===============================================
    // creates our simulation board
    char simBoard[size][size];
    // generates a new board--all trees, one burning in the middle
    initializeSimBoard(treeDensity, proportionBurning, size, simBoard);
    
    
    // we're all set up we can begin simulating
    // if we have a set number of iterations: run that procedure
    if(numberOfIterations != -1)
    {
        runIterations(numberOfIterations, size, simBoard);
    }
    // else we run the inifinite, cursor controlled system
    else
    {
        runIndeterminate(size, simBoard);
    }
    
    // prints our board (for my sanity)
    printBoard(size, simBoard);

    return EXIT_SUCCESS;
}
