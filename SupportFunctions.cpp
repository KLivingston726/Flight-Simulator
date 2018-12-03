#include "SupportFunctions.h"

/**
\file SupportFunctions.cpp
\brief Functions for easier generation of pseudo-random numbers.

\author    Don Spickler
\version   1.2
\date      3/3/2018

*/

/**
\brief Sets the seed of the random number generator to the clock value.

*/

void setSeed()
{
    setSeed(0);
}

/**
\brief Sets the seed of the random number generator.

\param s --- The seed of the generator if non-zero and the clock value if 0;

*/

void setSeed(int s)
{
    if (s == 0)
        srand((unsigned)time(0));
    else
        srand(s);
}

/**
\brief Returns a pseudo-random double in the interval [0, 1).

*/

double randf()
{
    return (double)rand()/RAND_MAX;
}

/**
\brief Returns a pseudo-random double in the interval [lb, ub).

\param lb --- Lower bound on the interval.
\param ub --- Upper bound on the interval.

*/

double randfRange(double lb, double ub)
{
    return lb + (ub-lb)*(double)rand()/(double)RAND_MAX;
}

/**
\brief Returns a pseudo-random double in (-inf, inf) which uses a
normal random variate generator with mean m, standard deviation s.

\param m --- The mean.
\param s --- The standard deviation.

*/

double randGauss(double m, double s)
{
    double x1, x2, w, y1;
    static double y2;
    static int use_last = 0;

    if (use_last)
    {
        y1 = y2;
        use_last = 0;
    }
    else
    {
        do
        {
            x1 = 2.0 * randf() - 1.0;
            x2 = 2.0 * randf() - 1.0;
            w = x1 * x1 + x2 * x2;
        }
        while (w >= 1.0);

        w = sqrt((-2.0*log(w))/w);
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }

    return m+y1*s;
}
