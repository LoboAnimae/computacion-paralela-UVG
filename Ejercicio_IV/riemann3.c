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
#define THREADS 8
double f(double x); // La funcion a integrar
double trapezoides(double a, double b, int n);
int main(int argc, char *argv[])
{
    double integral;
    double a = A, b = B;
    int n = N;
    double h;
    int thread_count = THREADS;
    if (argc > 1)
    {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
        thread_count = strtol(argv[3], NULL, 10);
    }
    //---- Aproximacion de la integral
    double *subAreas = (double *)malloc(thread_count * sizeof(double));
#pragma omp parallel num_threads(thread_count)
    {
        integral = trapezoides(a, b, n);
        subAreas[omp_get_thread_num()] = integral;
    }
    for (int i = 0; i < thread_count; i++)
    {
        integral += subAreas[i];
        printf("[INFO] Integral: %.2f + %.2f = %.2f\n", integral - subAreas[i], subAreas[i], integral);
    }
    printf("Con n = %d trapezoides, nuestra aproximacion \n", n);
    printf("de la integral de %f a %f es = %.10f\n", a, b, integral);
    return 0;
} /*main*/
//------------------------------------------
// trapezoides
//
// Estimar la integral mediante sumas de Riemann
// Input: a,b,n,h
// Output: integral
//------------------------------------------
double trapezoides(double a, double b, int n)
{
    double n_local, a_local, b_local, my_result, integral, h, x;
    int thread_count = omp_get_num_threads(),
        my_rank = omp_get_thread_num(),
        k;

    n_local = n / thread_count;
    //---- Ancho de cada trapezoide
    h = (b - a) / n;
    a_local = a + my_rank * n_local * h;
    b_local = a_local + n_local * h;
    my_result = (f(a_local) + f(b_local)) / 2.0;
    // my_result = 0;
    printf("[INFO] Thread %d: trapezoides(%f, %f, %f, %f)\n", my_rank, a_local, b_local, n_local, h);
    //---- Valor inicial de la integral (valores extremos)
    integral = (f(a) + f(b)) / 2.0;
    for (k = 1; k <= n_local - 1; k++)
    {
        x = a_local + k * h;
        my_result += f(x);
    }

    my_result = my_result * h;
    return my_result;
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
