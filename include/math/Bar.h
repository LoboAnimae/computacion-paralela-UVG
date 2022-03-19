#ifndef BAR_PROJECT_H
#define BAR_PROJECT_H

typedef struct Deltas
{
    double delta_t;
    double delta_x;
} Deltas;

typedef struct Times
{
    long double t_j;
    long double t_adjacent_1;
    long double t_adjacent_2;
} Times;

typedef struct Sequence
{
    long double t_0;
    long double t_left;
    long double t_right;
} Sequence;

typedef struct Params
{
    long double err;
    long int n;
    double c;
    double l;
    int thread_count;
    long long iterations;
} Params;

/**
 * @brief Calculates the temperatures of a metal bar
 *
 * @param seq Allows for the left and right temps to be known
 * @param param Allows for the misc params to be passed
 * @param delta Allows for the deltas to be known
 * @return long double*
 */
long double *caulculate_temperatures(Sequence *seq, Params *param, Deltas *delta);

/**
 * @brief Gets the new temperature
 *
 * @param time The current time struct
 * @param delta a Delta struct
 * @param param a Params struct
 * @return long double
 */
long double obtain_temperature(Times *time, Deltas *delta, Params *param);
#define DEFAULT_REMAINING_ITERATIONS 10000000000

#endif