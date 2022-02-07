#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <strings.h>

void openmp_hello(int *age, int thread_num, int isEven);

int main(int argc, char *argv[])
{
    // Initialize variables
    // The number of threads wanted
    int threadCount;
    // The age of the person that's using the program
    int age;
    // The number of threads that are available
    int threadNumber;

    // Config
    threadCount = 8;
    threadNumber = 0;

    // Check if parameters were passed in the CLI
    if (argc > 0)
    {
        age = strtol(argv[1], NULL, 10);
    }
    else
    {
        age = 23;
        printf("No age given, defaulting to %d\n", age);
    }

#pragma omp parallel num_threads(threadCount)
    {
        int thread_num = omp_get_thread_num();
        // Thread num will not be borrowed, but age will be
        openmp_hello(&age, thread_num, thread_num % 2);
        threadNumber = omp_get_num_threads();
    }
    printf("There were %d threads running in this program!", threadNumber);
    return 0;
}

void openmp_hello(int *age, int thread_num, int isEven)
{
    isEven
        ? printf("Feliz cumpleaños número %d! (hilo %d)\n", *age, thread_num)
        : printf("Saludos desde el hilo %d!\n", thread_num);
}
