/*----------------------------------------------
 * riemann.c - calculo de area bajo la curva
 *----------------------------------------------
 * Sumas de Riemann para calcular la integral f(x)
 *
 * Date:  2021-09-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define A 1
#define B 40
#define N 10e6

double f(double x); // La funcion a integrar
double trapezoides(double a, double b, int n, double h, double *globalResult);

int main(int argc, char *argv[])
{
    double integral;
    double a = A, b = B;
    int n = N, thread_count = 1;
    double h;

    if (argc > 1)
    {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
        thread_count = strtol(argv[3], NULL, 10);
    }

    //---- Aproximacion de la integral
    // h = (b-a)/n;
    double batchSize = (b - a) / thread_count;
    printf("[INFO] batch size: %f\n", batchSize);
    h = (b - a) / n;
    printf("[INFO] h: %f\n", h);
    double *integrals = (double *)malloc(thread_count * sizeof(double));
    double global_result = 0;
#pragma omp parallel num_threads(thread_count)
    {
        int thread_num = omp_get_thread_num();
        int differentiator = (thread_num == 0) ? 0 : 1;
        double localA = a * ((thread_num * batchSize) + 1);
        double localB = localA + batchSize + differentiator;
        if (localB > b)
            localB = b;
        int localN = n / thread_count;
        integrals[thread_num] = trapezoides(localA, localB, localN, h, &global_result);
    }
    printf("[INFO] Trapezoid Number: %f\n", n);
    printf("[INFO] Area: %f\n", global_result);

    return 0;
} /*main*/

//------------------------------------------
// trapezoides
//
// Estimar la integral mediante sumas de Riemann
// Input: a,b,n,h
// Output: integral
//------------------------------------------
double trapezoides(double a, double b, int n, double h, double *globalResult)
{
    double integral, local_result = 0;
    int k;

    //---- Ancho de cada trapezoide
    // h = (b-a)/n;
    // printf("[INFO] h: %f\n", h);
    //---- Valor inicial de la integral (valores extremos)
    integral = (f(a) + f(b)) / 2.0;

    for (k = 1; k <= n - 1; k++)
    {
        integral += f(a + k * h);
    }
    double suma_local = integral * h;
#pragma omp critical
    {
        *globalResult += suma_local;
    }
    return local_result;
} /*trapezoides*/

//------------------------------------------
// f
//
// Funcion a ser integrada
// Input: x
//------------------------------------------
double f(double x)
{
    double return_val;

    return_val = x * x;

    return return_val;
} /*f*/
