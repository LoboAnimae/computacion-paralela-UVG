/* Programa que suma todos los elementos de un arreglo secuencialmente.
 **/
#include <stdio.h>
#include <stdlib.h>
// #include <mpi.h>
#define max_rows 100000
#define send_data_tag 2001
#define return_data_tag 2002
int array[max_rows];
int array2[max_rows];
int main(int argc, char **argv)
{
    long int sum, partial_sum;
    // MPI_Status status;
    int i, num_rows;
    // printf("ingrese la cantidad de numeros a sumar: ");
    // scanf("%i", &num_rows);
    num_rows = 10;
    if (num_rows > max_rows)
    {
        printf("Muchos elementos a sumar.\n");
        exit(1);
    }
    /* init el array */
    for (i = 0; i < num_rows; i++)
    {
        array[i] = i + 1;
    }
    // SUM secuencialmente
    for (i = 0; i < num_rows; i++)
    {
        sum += array[i];
    }
    printf("La suma es: %ld\n", sum);
}