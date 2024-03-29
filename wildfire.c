///
/// File: wildfire.c
///
/// Description: Creates an instance of a wildfire simulation using the rules
///              provided in [Shiflet] and a few modifications.
///
/// @author: kjb2503 : Kevin Becker
///
// // // // // // // // // // // // // // // // // // // // // // // // // // //

#define _BSD_SOURCE
#include <unistd.h> // getopt
#include <stdio.h> // fprintf
#include <stdlib.h> // rand(), EXIT_SUCCESS
#include <time.h> //time
#include "run_wildfire.h" // runIterataions, runIndeterminate
#include "wildfire_values.h" // all of the shared values

// some things we need to use globally which will not change once set
static int treeDensity, proportionBurning, probability;


///
/// Function: printUsageMsg
///
/// Description: Displays the usage of the program.
///
/// @param *cmdUsed  The command used to start the program (for completely
///                  accurate usage).
///
static void printUsageMsg(const char *cmdUsed)
{
    // prints our usage to stderr
    fprintf(stderr, "usage: %s [-pN] size probability treeDensity proportionBurning\
    \nThe -pN option tells the simulation to print N cycles and stop.\
    \nThe probability is the probability a tree will catch fire.\n", cmdUsed);
    // exits our program with status of EXIT_FAILURE
    exit(EXIT_FAILURE);
}


///
/// Function: requiredArgumentError
///
/// Description: Prints out an argument error for the always required arguments
///              if one is encountered in main.
///
/// @param *cmdUsed  The command used to start the program (for printUsageMsg).
/// @param name  The name of the argument which was bad.
/// @param value  The value given by the user.
/// @param low  The low which the argument must be greater than or equal to.
/// @param high  The high which the argument must be greater than or equal to.
///
static void requiredArgumentError(const char *cmdUsed, const char *name,
                          int value, int low, int high)
{
    // prints our error
    fprintf(stderr, "The %s (%d) must be an integer in [%d...%d].\n",
        name, value, low, high);
    // prints our usage message and exits the program
    printUsageMsg(cmdUsed);
}


///
/// Function: iterationsArgumentError
///
/// Description: Prints out an argument error if the number of iterations is no
///              good.
///
/// @param *cmdUsed  The command used to start the program (for printUsageMsg).
/// @param *whichErr  Which type of error was encountered (invalid/negative).
///
static void printFlagError(const char *cmdUsed, const char *whichErr)
{
    // prints our error
    fprintf(stderr, "The -pN argument was %s.\n", whichErr);
    // prints our usage message and exits the program
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
static void initializeSimBoard(int size, char simBoard[][size])
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
    int totalTrees = (size * size) * (treeDensity / 100.0);
    // determines the number of burning trees
    int totalBurningTrees = totalTrees * (proportionBurning / 100.0);

    // dispurses our trees
    while(totalTrees > 0)
    {
        // determines a random location
        row = rand() % size;
        col = rand() % size;

        // if our space is already filled with something and subtract from
        // the total trees remaining
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
            simBoard[row][col] = BURNING_VALUE;
            --totalBurningTrees;
        }
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
    // if we have less than 5 arguments, it is a usage issue
    if(argc < 5)
        printUsageMsg(argv[0]);

    // if argc more than 6, might be a p flag issue or just a usage issue
    if(argc > 6)
    {
        // if p is included, this means we have an invalid p
        if(getopt(argc, argv, "p") == 'p')
            printFlagError(argv[0], "invalid");
        else
            // prints our usage
            printUsageMsg(argv[0]);
    }

    // we set it to -1 (changed if we do not want to go forever)
    int opt, size, numberOfIterations = -1;

    // this is always run just to make sure we have no excess flags
    while((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch(opt)
        {
            // if our flag is present (and the correct one)
            // pull in our number of iterations to complete
            case 'p':
                numberOfIterations = strtol(optarg, NULL, 10);
                // if p is negative, that's no good
                if(numberOfIterations < 0)
                    printFlagError(argv[0], "negative");
                // if our flag isn't the first included argument, it's wrong
                // NOTE: this also means that if program is run using:
                // wildfire -p N size prob dens prop
                // it is also incorrect (N must be included next to flag)
                if(optind > 2)
                    printFlagError(argv[0], "invalid");
                break;
            default:
                printUsageMsg(argv[0]);
        }
    }

    // last check before starting, if we have 6 arguments and no changes made
    // to numberOfIterations, OR if we do have a proper flag, but not enough
    // remaining arguments to start, we have an issue and cannot move forward
    if((argc == 6 && numberOfIterations == -1) || (argc-optind < 4))
        printUsageMsg(argv[0]);

    // reads in our size, prob, density and proportion values to variables
    size = strtol(argv[optind], NULL, 10);
    probability = strtol(argv[optind+1], NULL, 10);
    treeDensity = strtol(argv[optind+2], NULL, 10);
    proportionBurning = strtol(argv[optind+3], NULL, 10);

    // builds a format string
    // it's a little funky here, but our input parameters remain constant so
    // we don't need to continually keep putting them in.
    // the maximum number of changes per iteration is 1600 so there are two extra
    // spaces printed
    char statusFormatString[] = "cycle %s, size %d, probability %.2f, density "
                          "%.2f, proportion %.2f, changes %s \n";
    // formats our format string so that everything is print correctly
    // we are printing to our format string using our format string, the %s will be
    // the two non-constant integers changing between each iteration
    sprintf(statusFormatString, statusFormatString, "%d", size, (probability/100.0),
            (treeDensity/100.0), (proportionBurning/100.0),"%d");

    // goes through and checks each command line argument for correct formatting
    if(size < 5|| size > 40)
        requiredArgumentError(argv[0], "size", size, 5, 40);
    if (probability < 0 || probability > 100)
        requiredArgumentError(argv[0], "probability", probability, 0, 100);
    if (treeDensity < 0 || treeDensity > 100)
        requiredArgumentError(argv[0], "density", treeDensity, 0, 100);
    if (proportionBurning < 0 || proportionBurning > 100)
        requiredArgumentError(argv[0], "proportion", proportionBurning, 0, 100);

    // BOARD GENERATION SEQUENCE ===============================================

    // creates our uninitiailized simulation board
    char simBoard[size][size];

    // generates a new board--all trees, one burning in the middle
    initializeSimBoard(size, simBoard);

    // we're all set up, all that's left is actually simulating!
    runWildfire(statusFormatString, numberOfIterations, probability, size, simBoard);

    // returns EXIT_SUCCESS upon completion
    return EXIT_SUCCESS;
}

