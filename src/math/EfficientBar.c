#include "math/Bar.h"
#include "math.h"
#include <stdlib.h>
#include <omp.h>
#include "math/EfficientBar.h"

long long *generate_intervals(long long, long long);

/**
 * Discussed options for parallelization:
 * Producer + consumer (we use two consumers and avoid frontier value exchange)
 * Tasks (as in pseudocode)
 * Sections
 * **/
long double *calculate_temperatures(Sequence *seq, Params *param)
{
    //     omp_set_num_threads(param->thread_count); // Threads to test with
    //     long long ll_remaining_iterations = param->iterations;
    //     long double ld_current_error = 0, // Check to see if we stop the loop
    //         ld_new_temperature,           // New calculated temperature
    //         *ldp_temperatures;            // All temperatures
    //     Deltas delta;
    //     Times time;

    // #pragma region Heap Memory
    //     ldp_temperatures = (long double *)malloc(sizeof(long double) * param->n);
    // #pragma endregion
    // // Array initialization
    // #pragma omp parallel for num_threads(6)
    //     for (int i = 0; i < param->n; i++)
    //     {
    //         ldp_temperatures[i] = seq->t_0;
    //     }

    //     ldp_temperatures[0] = seq->t_left;             // TL
    //     ldp_temperatures[param->n - 1] = seq->t_right; // TR
    //     delta.delta_x = 1 / param->n;
    //     delta.delta_t = 0.3 * delta.delta_x * delta.delta_x / param->c;

    //     while (ld_current_error > param->err && ll_remaining_iterations-- > 0)
    //     {
    //         for (int j = 1; j < param->n; ++j)
    //         {
    //             time.t_j = ldp_temperatures[j];
    //             time.t_adjacent_1 = ldp_temperatures[j - 1];
    //             time.t_adjacent_2 = ldp_temperatures[j + 1];
    //             ld_new_temperature = obtain_temperature(&time, &delta, param);
    //             ld_current_error = (ld_current_error < fabsl(ldp_temperatures[j] - ld_new_temperature)) ? fabsl(ldp_temperatures[j] - ld_new_temperature) : ld_current_error;
    //             ldp_temperatures[j] = ld_new_temperature;
    //         }
    //     }

    //     return ldp_temperatures;
    int threads = param->thread_count;
    omp_set_num_threads(threads);
    int N;           // Number of discreet intervals
    long double t_0, // Initial temperature
        t_l,         // Left temperature
        t_r;         // Right temperature
    double c = param->c;

    Deltas delta = {
        .delta_x = 1 / N,
    };

    delta.delta_t = 0.3 * delta.delta_x * delta.delta_x / c;

    N = param->n;
    t_0 = seq->t_0;
    t_l = seq->t_left;
    t_r = seq->t_right;

    // Divide the domains into discreet intervals
    long long *borders = generate_intervals(N, threads);

#pragma omp private()
    {
        int thread_id = omp_get_thread_num();
        int border_right = borders[thread_id];
    }
}

/**
 * @brief Returns the RIGHT (ending) INTERVALS for each thread. The interval size can be done as n + 1 - n
 *
 * @param total
 * @param count
 * @return long long*
 */
long long *generate_intervals(long long total, long long count)
{
    // Allows us to create a dynamic array of intervals to be used in the parallelization
    long long *intervals = (long long *)malloc(sizeof(long long) * count);
    // If it is a direct division
    if (total % count == 0)
    {
        long long interval = total / count;
        for (int i = 0; i < count; i++)
        {
            intervals[i] = interval;
        }
    }
    else
    {

        // If it is not, then grab the closest full division
        long long overflow = total % count;

        int i = 0;
        // While there are still overflows, sum it to each interval
        while (overflow > 0)
        {
            intervals[i]++;  // Add one to the current interval
            overflow--;      // Take out one overflow
            i = ++i % count; // Make it so that the interval doesn't go out of bounds
        }
    }

    // In any case, return the new intervals
    return intervals;
}

long double obtain_temperature(Times *time, Deltas *delta, Params *param)
{
    return time->t_j * (1 + (param->c * delta->delta_t / delta->delta_x) * (time->t_adjacent_1 - 2 + time->t_adjacent_2));
}
