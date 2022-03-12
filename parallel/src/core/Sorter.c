
#include "core/Sorter.h"

int *quicksort(int *array, int lo, int hi)
{
    if (lo > hi)
        return array;

    int pivot = array[lo];
    int i = lo;
    int j = hi;
    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;

        if (i <= j)
        {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    // Recursion

    // #pragma omp parallel sections
    // {
    // #pragma omp section
    // {
    quicksort(array, lo, j);
    // }
    // #pragma omp section
    // {
    quicksort(array, i, hi);
    // }
    // }
}