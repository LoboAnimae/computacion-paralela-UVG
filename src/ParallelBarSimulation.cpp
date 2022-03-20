#include <iostream>
#include <cmath>
#include <string>       // string
#include <omp.h>        // OpenMP
using namespace std;    // std::

double get_numeric_user_input(const string& message);
long double max_error(long double A[], long double B[], int i, int j);


// main function
int main(int argc, char* argv[]) {

    // Base parameters for simulation
    omp_set_num_threads(24);
    int n = 30000;
    double length = 10;
    double C = 2;
    double t_0 = 10;
    double t_left = 50;
    double t_right = 1000;
    double error = 0.1;

    // Arrays to be used in calculations
    long double ldp_temperatures_old[n];
    long double ldp_temperatures_new[n];

    // Deltas
    double delta_x = length / n;
    double delta_t = 0.3 * delta_x * delta_x / C;

    // Array initializations
    for(int i =1; i<n-1; i++) {
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

    // Parallel region
    #pragma omp parallel
    {
        // Infinite loop
        while(true) {

            // Error restart
            double max_error = 0;

            // Parallel for chunk size = 16 (long double), reduction with max error to stop the while when end condition is reached
            #pragma omp parallel for schedule(static, 16) shared(b, a, ldp_temperatures_old, ldp_temperatures_new) reduction(max: max_error)
            for (i = 1; i < n - 1; i++) {
                // New temperature calculation with a different factorization
                ldp_temperatures_new[i] = ldp_temperatures_old[i] * b + a * (ldp_temperatures_old[i - 1] + ldp_temperatures_old[i + 1]);
                // Maximum error calculation
                max_error = max_error > fabs(ldp_temperatures_old[i] - ldp_temperatures_new[i]) ? max_error : fabs(ldp_temperatures_old[i] - ldp_temperatures_new[i]);
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
    cout<<"TOTAL TIME:"<< omp_get_wtime() - initial_time<<endl;
    cout<<"THE BAR IS COMPLETELY CALCULATED! Temperatures are as follows (L-R): "<<ldp_temperatures_new[0]<<" "<<ldp_temperatures_old[1]<<" "<<ldp_temperatures_old[2];
}