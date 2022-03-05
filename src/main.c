#include "core/Screen.h"
#include "main.h"
#include "core/ConsoleController.h"
#include "core/ErrorHandling.h"
#include "core/Screen.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{

    // Create a screen
    Screen *menu = create_screen();
    Screen *names = create_screen();
    ScreenComponent *titleComponent = create_screen_component();
    ScreenComponent *fileComponent = create_screen_component();
    ScreenComponent *bodyComponent = create_screen_component();
    ScreenComponent *namesComponent = create_screen_component();
    ScreenComponent *footerComponent = create_screen_component();

    add_component(menu, titleComponent);
    add_component(menu, fileComponent);
    add_component(menu, bodyComponent);
    add_component(menu, footerComponent);

    add_component(names, titleComponent);
    add_component(names, namesComponent);
    add_component(names, footerComponent);

    add_line(titleComponent, "Proyecto 1\n===================\n\n");
    add_line(bodyComponent, "\tMenu Principal\n\t_________________\n\n");
    add_line(bodyComponent, "\tPor favor seleccione una opcion\n");
    add_line(bodyComponent, "\t\n===================\n\t1. Nombres\n\t2. Salir");
    add_line(namesComponent, "\tNombres\n\t_________________\n\n");
    add_line(namesComponent, "\tNombre\tApellido\t17652\n");
    add_line(namesComponent, "\tAndrés\tQuan-Littow\tCarnet\n");
    add_line(namesComponent, "\tPablo\tRuiz\t\tCarnet\n");
    add_line(namesComponent, "\tMichael\tChan\t\tCarnet\n");
    add_line(namesComponent, "\n\n===================\n\t1. Atrás");

    add_line(footerComponent, "\n\n====================\n>>> ");
    refresh(menu, NULL);

    Screen *currentScreen = menu;
    refresh(currentScreen, NULL);
    int currentScreenIndex = 0;
    while (1)
    {
        int option;
        scanf("%d", &option);
        if (currentScreenIndex == 0)
        {
            if (option == 1)
            {
                currentScreen = names;
                currentScreenIndex = 1;
                refresh(currentScreen, NULL);
            }
            else if (option == 2)
            {
                break;
            }
        }
        else if (currentScreenIndex == 1)
        {
            if (option == 1)
            {
                currentScreen = menu;
                currentScreenIndex = 0;
                refresh(currentScreen, NULL);
            }
        }
        else
        {
            printf("[ERROR] [BUG] THE SCREEN INDEX IS OUT OF RANGE\n");
            return -1;
        }
    }

    return 0;
}

/**
 * Calculates the temperatures of a metal bar
 * **/
long double * calculate_temperatures(long double err, long int n, long double t_0, long double t_left, long double t_right double l, double c) {
    int remaining_iterations = 10000000000;
    long double current_error;                       // Check to see if we stop the loop
    long double new_temperature;                     // New calculated temperature
    long double temperatures[n];                     // All temperatures
    temperatures[0] = t_left;
    temperatures[1] = t_0;
    temperatures[2] = t_0;
    temperatures[n-1] = t_right;

    while (current_error > err || remaining_iterations == 0) {
        for (int j = 1; j < n; j++) {
            // Computes the new temperature, adds it to the error calculation, and updates temperature list
            new_temperature = obtain_temperature(temperatures[j], c, (0.1 * pow(delta_x, 2)) / c, l / n, temperatures[j-1], temperatures[j+1]);
            current_error += fabsl(temperatures[j] - new_temperature);
            temperatures[j] = new_temperature;
        }
        current_error /= n;
        remaining_iterations--;
    }
    return temperatures;
}



/**
 * Gets the next temperature
 * **/
long double obtain_temperature(long double t_j, double c, double delta_t, double delta_x, long double t_adjacent_1, long double t_adjacent_2) {
    // Returns the new temperature
    return t_j + ((c * delta_t) / (pow(delta_x, 2))) * (t_adjacent_1 - 2 * t_j + t_adjacent_2);
}
