/* Programa que suma todos los elementos de un arreglo secuencialmente.
 **/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define max_rows 100000
#define send_data_tag 2001
#define return_data_tag 2002

int main(int argc, char **argv)
{
    long int sum, partial_sum;
    MPI_Status status;
    int i, num_rows;
    // printf("ingrese la cantidad de numeros a sumar: ");
    // scanf("%i", &num_rows);
    num_rows = max_rows;

    if (num_rows > max_rows)
    {
        printf("Muchos elementos a sumar.\n");
        exit(1);
    }

    MPI_Init(&argc, &argv);
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    // Rank 0 is producer
    if (rank == 0)
    {
        int sending_array[max_rows];
        // Rank 0 initializes the array
        for (i = 0; i < num_rows; i++)
        {
            sending_array[i] = i + 1;
        }
        int overflow = num_rows % num;
        int chunk_size = num_rows / num;
        for (int i = 1; i < num; ++i)
        {
            printf("Rank %d\n", i);
            int offset = (i - 1) * chunk_size;
            MPI_Send(&chunk_size, 1, MPI_INT, i, send_data_tag, MPI_COMM_WORLD);
            MPI_Send(&sending_array[offset], chunk_size, MPI_INT, i, send_data_tag, MPI_COMM_WORLD);
        }
        int accumulator = 0;
    }
    // Other ranks are consumers
    else
    {
        int receiving_array[max_rows];
        int chunk_size;
        // Will wait for
        // Wait for information from Rank 0
        MPI_Recv(&chunk_size, 1, MPI_INT, 0, send_data_tag, MPI_COMM_WORLD, &status);
        printf("Rank %d received a size of %d", rank, chunk_size);
        MPI_Recv(receiving_array, chunk_size, MPI_INT, 0, send_data_tag, MPI_COMM_WORLD, &status);
        /*
        =======================================
        --- 1 --- --- 2 --- --- 3 --- --- 4 ---
        */
        int local_accumulator = 0;

        // For here

        // MPI_Send(local_accumulator, 1, MPI_INT, 0, return_data_tag, MPI_COMM_WORLD);
    }
    /* init el array */

    // SUM secuencialmente
    // for (i = 0; i < num_rows; i++)
    // {
    //     sum += array[i];
    // }

    MPI_Finalize();
    printf("La suma es: %ld\n", sum);
}