#include "math/Bar.h"
#include "math.h"
#include <stdlib.h>

long double *calculate_temperatures(Sequence *seq, Params *param)
{
    long long ll_remaining_iterations = DEFAULT_REMAINING_ITERATIONS;
    long double ld_current_error, // Check to see if we stop the loop
        ld_new_temperature,       // New calculated temperature
        *ldp_temperatures;        // All temperatures
    Deltas delta;
    Times time;

#pragma region Heap Memory
    ldp_temperatures = (long double *)malloc(sizeof(long double) * param->n);
#pragma endregion

    // Array initialization
    for (int i = 0; i < param->n; i++)
    {
        ldp_temperatures[i] = seq->t_0;
    }

    ldp_temperatures[0] = seq->t_left;             // TL
    ldp_temperatures[param->n - 1] = seq->t_right; // TR
    delta.delta_x = 1 / param->n;
    delta.delta_t = 0.3 * delta.delta_x * delta.delta_x / param->c;

    while (ld_current_error > param->err && ll_remaining_iterations-- > 0)
    {
        for (int j = 1; j < param->n; ++j)
        {
            time.t_j = ldp_temperatures[j];
            time.t_adjacent_1 = ldp_temperatures[j - 1];
            time.t_adjacent_2 = ldp_temperatures[j + 1];

            ld_new_temperature = obtain_temperature(&time, &delta, param);
            ld_current_error += fabsl(ldp_temperatures[j] - ld_new_temperature);
            ldp_temperatures[j] = ld_new_temperature;
        }
        ld_current_error /= param->n;
    }

    return ldp_temperatures;
}

long double obtain_temperature(Times *time, Deltas *delta, Params *param)
{
    return time->t_j + (param->c * delta->delta_t / delta->delta_x) * (time->t_adjacent_1 - 2 * time->t_j + time->t_adjacent_2);
}