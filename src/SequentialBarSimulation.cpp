#include <iostream>
#include <cmath>
#include <omp.h>        // OpenMP
using namespace std;    // std::

// main function
int main(int argc, char* argv[]) {

    int n = 10000;
    double length = 10;
    long double C = 2;
    double t_0 = 10;
    double t_left = 50;
    double t_right = 1000;
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
    for(int i =0; i<n; i++) {
        ldp_temperatures_old[i] = t_0;
        ldp_temperatures_new[i] = t_0;
        errors[i] = 0.0;
    }
    ldp_temperatures_old[0] = t_left;
    ldp_temperatures_new[0] = t_left;
    ldp_temperatures_old[n - 1] = t_right;
    ldp_temperatures_new[n - 1] = t_right;

    double initial_time = omp_get_wtime();

    while(true) {
        // We iterate through the whole bar
        for (int i = 1; i < n - 1; i++) {
            // New temperature calculation
            ldp_temperatures_new[i] = ldp_temperatures_old[i] + C * (delta_t / (delta_x * delta_x)) * (ldp_temperatures_old[i - 1] - 2 * ldp_temperatures_old[i] + ldp_temperatures_old[i + 1]);
            // Maximum error to stop the outer while loop
            attempted_error = fabs(ldp_temperatures_new[i] - ldp_temperatures_old[i]);
            if (attempted_error > current_error) {
                current_error = attempted_error;
            }
            // Updating the list
            ldp_temperatures_old[i] = ldp_temperatures_new[i];
        }
        // Once we reach the desired error, we break the loop
        if (current_error <= error) {
            break;
        }
        current_error = 0;
    }

    double final_time = omp_get_wtime();
    cout<<"TOTAL TIME:"<<final_time-initial_time<<endl;
    cout<<"THE BAR IS COMPLETELY CALCULATED! Temperatures are as follows (L-R):"<<ldp_temperatures_new[0]<<" "<<ldp_temperatures_old[1]<<" "<<ldp_temperatures_old[2];
}
