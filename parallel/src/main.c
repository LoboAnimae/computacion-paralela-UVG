#include "core/Generators.h"
#include "core/IO.h"
#include <stdio.h>
#include <stdlib.h>
#include "core/Sorter.h"
#include "omp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: main.o file_name number_of_numbers\n");
        exit(1);
    }

    char *file_name = argv[1];
    int N = atoi(argv[2]);

    double start_time = omp_get_wtime();
    printf("File: %s\nNumber of numbers: %d\n", file_name, N);
    printf("Generating random numbers...\n");
    int *randoms = generate_random_number_list(N);
    save_to_file(randoms, N, file_name, ',');
    free(randoms);
    char *file_contents = read_from_file(file_name);
    int file_size = 0;
    while (file_contents[file_size] != '\0')
    {
        file_size++;
    }
    int *numbers = split(file_contents, ',', file_size);
    printf("Sorting...\n");
    int threads = 8;
    omp_set_num_threads(threads);

    double time_now = omp_get_wtime();
#pragma omp parallel sections
    {
#pragma omp section
        {
            quicksort(numbers, 0, N / threads - 1);
        }
#pragma omp section
        {
            quicksort(numbers, N / threads, N - 1);
        }
    }

    double time_to_sort = omp_get_wtime() - time_now;
    char *sorted_file_name = "sorted.txt";
    save_to_file(numbers, N, sorted_file_name, ',');
    printf("Wrote to file %s\n", sorted_file_name);
    double total_time = omp_get_wtime() - start_time;
    printf("Program time: %f\n", total_time);
    printf("Sorting time: %f\n", time_to_sort);
    return 0;
}