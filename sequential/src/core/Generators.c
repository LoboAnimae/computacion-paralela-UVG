#include "core/Generators.h"
#include <stdlib.h>
#include <time.h>

int *generate_random_number_list(int N)
{
    int *random_number_list = (int *)malloc(N * sizeof(int));
    srand(time(0));
    for (int i = 0; i < N; ++i)
    {
        random_number_list[i] = rand() % N + 1;
    }
    return random_number_list;
}