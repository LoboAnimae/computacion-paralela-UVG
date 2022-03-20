#include <unistd.h> //std lib
#include <iostream>
#include <cmath>
#include <stdlib.h> // srand, rand
#include <fstream>  // fstream, ofstream, ifstream
#include <string>   // string
#include <time.h>   // time
#include <omp.h>
#include "parallel.h"
using namespace std; // std::

double get_numeric_user_input(const string &message);

// main function
int main(int argc, char *argv[])
{
    double start_time = omp_get_wtime();
    // Values for the calculation
    // int n = (int)get_numeric_user_input("Ingrese el numero de intervalos n");
    // double length = get_numeric_user_input("Ingrese la longitud de la barra");
    // long double C = get_numeric_user_input("Ingrese el valor de 'c' para la barra");
    // double t_0 = get_numeric_user_input("Ingrese la temperatura de la barra");
    // double t_left = get_numeric_user_input("Ingrese la temperatura izquierda de la barra");
    // double t_right = get_numeric_user_input("Ingrese la temperatura derecha de la barra");
    // double error = get_numeric_user_input("Ingrese el error que desea obtener");

    int n = 10000;
    double length = 50;
    long double C = 5;
    double t_0 = 50;
    double t_left = 51;
    double t_right = 100;
    double error = 0.1;
    // Errors:
    long double current_error = 0;
    long double attempted_error = 0;

    // Arrays to be used in calculations
    long double ldp_temperatures_old[n];
    long double ldp_temperatures_new[n];
    long double errors[n];

    // Deltas
    double delta_x = length / n;
    double delta_t = 0.3 * delta_x * delta_x / C;

    // Array initializations

    for (int i = 0; i < n; i++)
    {
        ldp_temperatures_old[i] = t_0;
        ldp_temperatures_new[i] = t_0;
        errors[i] = 0.0;
    }
    ldp_temperatures_old[0] = t_left;
    ldp_temperatures_new[0] = t_left;
    ldp_temperatures_old[n - 1] = t_right;
    ldp_temperatures_new[n - 1] = t_right;

    while (true)
    {
        cout << "Calculating error" << current_error << " " << error << endl;
        for (int i = 1; i < n - 1; i++)
        {
            ldp_temperatures_new[i] = ldp_temperatures_old[i] + C * (delta_t / (delta_x * delta_x)) * (ldp_temperatures_old[i - 1] - 2 * ldp_temperatures_old[i] + ldp_temperatures_old[i + 1]);
            attempted_error = fabs(ldp_temperatures_new[i] - ldp_temperatures_old[i]);
            cout << "Current error" << attempted_error << endl;
            if (attempted_error > current_error)
            {
                current_error = attempted_error;
            }
            ldp_temperatures_old[i] = ldp_temperatures_new[i];
        }
        if (current_error <= error)
        {
            break;
        }
        current_error = 0;
    }
    cout << "THE BAR IS COMPLETELY CALCULATED! Temperatures are as follows (L-R):";
    for (int i = 0; i < n; i++)
    {
        cout << ldp_temperatures_new[i] << endl;
    }

    cout << "The time taken to calculate the bar is: " << omp_get_wtime() - start_time << " seconds" << endl;
    double parallel_start_time = omp_get_wtime();
    ParallelBar *bar = new ParallelBar((long long)0, t_left, t_right, (double)C, (int)n, (int)delta_t, 8);
    bar->run();
    cout << "The time taken to calculate the bar in parallel is: " << omp_get_wtime() - parallel_start_time << " seconds" << endl;
    return 0;
}

double get_numeric_user_input(const string &message)
{
    bool valid = false;
    string input;
    double numeric_input;
    char *e;
    while (!valid)
    {
        cout << message << endl;
        cin >> input;
        if (input.find_first_not_of("1234567890.-") != string::npos)
        {
            cout << "invalid number: " << input << endl;
        }
        else
        {
            valid = true;
            numeric_input = strtod(input.c_str(), &e);
        }
    }
    return numeric_input;
}
