#include <iostream>
#include <cmath>
#include <stdlib.h>     // srand, rand
#include <string>       // string
#include <omp.h>        // OpenMP
using namespace std;    // std::

double get_numeric_user_input(const string& message);


// main function
int main(int argc, char* argv[]) {
    omp_set_num_threads(24);

    // Values for the calculation
    int n = (int) get_numeric_user_input("Ingrese el numero de intervalos n");
    double length = get_numeric_user_input("Ingrese la longitud de la barra");
    double C = get_numeric_user_input("Ingrese el valor de 'c' para la barra");
    double t_0 = get_numeric_user_input("Ingrese la temperatura de la barra");
    double t_left = get_numeric_user_input("Ingrese la temperatura izquierda de la barra");
    double t_right = get_numeric_user_input("Ingrese la temperatura derecha de la barra");
    double error = get_numeric_user_input("Ingrese el error que desea obtener");


    // Arrays to be used in calculations
    long double ldp_temperatures_old[n];
    long double ldp_temperatures_new[n];

    // Deltas
    double delta_x = length / n;
    double delta_t = 0.3 * delta_x * delta_x / C;

    // Array initializations
    for (int i = 1; i < n - 1; i++) {
        ldp_temperatures_old[i] = t_0;
        ldp_temperatures_new[i] = t_0;
    }
    ldp_temperatures_old[0] = t_left;
    ldp_temperatures_new[0] = t_left;
    ldp_temperatures_old[n - 1] = t_right;
    ldp_temperatures_new[n - 1] = t_right;

    double initial_time = omp_get_wtime();

    // Constants used in temperature calculation
    double a = C * delta_t / (delta_x * delta_x);
    double b = 1 - 2 * a;

    // Iterator
    int i;

    // Parallel region, 24 threads
    #pragma omp parallel
    {
        // Infinite loop
        while (true) {

            // Error restart
            double max_error = 0;

            // Parallel for chunk size = 16 (long double), reduction with max error to stop the while when end condition is reached
            #pragma omp parallel for schedule(static, 16) shared(b, a, ldp_temperatures_old, ldp_temperatures_new) reduction(max: max_error)
            for (i = 1; i < n - 1; i++) {
                // New temperature calculation with a different factorization
                ldp_temperatures_new[i] =
                        ldp_temperatures_old[i] * b + a * (ldp_temperatures_old[i - 1] + ldp_temperatures_old[i + 1]);
                // Maximum error calculation
                max_error = max_error > fabs(ldp_temperatures_old[i] - ldp_temperatures_new[i]) ? max_error : fabs(
                        ldp_temperatures_old[i] - ldp_temperatures_new[i]);
                // Array update
                ldp_temperatures_old[i] = ldp_temperatures_new[i];
            }

            // If end condition has been reached
            if (max_error <= error) {
                break;
            }
        }
    }

    // Total time and initial bar temperatures
    cout << "TOTAL TIME:" << omp_get_wtime() - initial_time << endl;
    cout << "THE BAR IS COMPLETELY CALCULATED! Temperatures are as follows (L-R): " << ldp_temperatures_new[0] << " "
         << ldp_temperatures_old[1] << " " << ldp_temperatures_old[2];
}



    double get_numeric_user_input(const string& message) {
    string input;   // Input
    char *e;        // End pointer for string to double
    while (true) {
        cout<<message<<endl;
        cin>>input;
        // We check if the string contains only numeric characters
        if( input.find_first_not_of("1234567890.-") != string::npos ) {
            cout << "invalid number: " << input << endl;
        } else {
            return strtod(input.c_str(), &e);
        }
    }
}