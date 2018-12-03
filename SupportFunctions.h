#ifndef SUPPORTFUNCTIONS_H_INCLUDED
#define SUPPORTFUNCTIONS_H_INCLUDED

#include <stdlib.h>
#include <math.h>
#include <ctime>

/**
\file SupportFunctions.h

\brief Header file for SupportFunctions.cpp

\author    Don Spickler
\version   1.1
\date      Written: 4/21/2016  <BR> Revised: 4/21/2016

*/

void setSeed();
void setSeed(int s);
double randf();
double randfRange(double lb, double ub);
double randGauss(double m, double s);

#endif // SUPPORTFUNCTIONS_H_INCLUDED
