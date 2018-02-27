#ifndef RUN_WILDFIRE
#define RUN_WILDFIRE

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
                 int probability, int size, char simBoard[][size]);

#endif
