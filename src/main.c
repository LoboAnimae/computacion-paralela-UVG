#include "core/Screen.h"
#include "main.h"
#include "core/ConsoleController.h"
#include "core/ErrorHandling.h"
#include "core/Screen.h"
#include "core/DataStructures.h"
#include <stdlib.h>
#include <omp.h>
#include "core/String.h"

#define SEPARATOR "\n=====================================\n"
#define SUBSEPARATOR "\n____________________________________\n"

int check_for_file_loaded(ScreenComponent *, int *, int *);
int change_screen(Screen *currentScreen, Screen *new_screen, int *index_pointer, int index, int with_refresh);
int handle_names();

int main(int argc, char *argv[])
{
    int file_loaded_message_id = -1;
    int file_loaded = 0;
    // Create a screen
    Screen *menu = create_screen();
    ScreenComponent *titleComponent = create_screen_component(NULL);
    add_component(menu, titleComponent);
    ScreenComponent *fileComponent = create_screen_component(menu);
    ScreenComponent *bodyComponent = create_screen_component(menu);
    ScreenComponent *footerComponent = create_screen_component(NULL);
    add_component(menu, footerComponent);

#pragma omp parallel shared(file_loaded_message_id, menu, titleComponent, fileComponent, bodyComponent, footerComponent, file_loaded)
    {
#pragma omp task
        {
            add_line(titleComponent, "Proyecto 1", GREEN);
            add_line(titleComponent, SEPARATOR, WHITE);
            add_line(fileComponent, "Archivo: ", WHITE);
            file_loaded_message_id = add_line(fileComponent, "NO CARGADO\n", RED);
            add_line(fileComponent, SEPARATOR, WHITE);
            add_line(bodyComponent, "\tMenu Principal\t", WHITE);
            add_line(bodyComponent, SUBSEPARATOR, WHITE);
            add_line(bodyComponent, "\n\tPor favor seleccione una opción\n", WHITE);
            add_line(bodyComponent, "\t1. Nombres\n\t2. Cargar Archivo de Configuración\n\t3. Salir", WHITE);
            add_line(footerComponent, "\n\n====================\n>>> ", WHITE);

            Screen *currentScreen = menu;
            int currentScreenIndex = 0;
            clear_screen(NULL);

            // TODO: Make a screen struct so that screens can be looked up by name
            refresh(currentScreen, NULL);
            while (1)
            {
                int option;
                scanf("%d", &option);
                if (currentScreenIndex == 0)
                {
                    if (option == 1)
                    {
                        handle_names();
                    }
                    else if (option == 2)
                    {
                        file_loaded = 1;
                        update_line(fileComponent, file_loaded_message_id, "CARGADO\n", GREEN, NULL);
                        refresh(menu, NULL);
                    }
                    else if (option == 3)
                    {
                        break;
                    }
                }
                else if (currentScreenIndex == 1)
                {
                    if (option == 1)
                    {
                        change_screen(currentScreen, menu, &currentScreenIndex, 0, 1);
                    }
                }
                else
                {
                    printf("[ERROR] [BUG] THE SCREEN INDEX IS OUT OF RANGE\n");
                    break;
                }
            }
        }
    }
    return 0;
}

int check_for_file_loaded(ScreenComponent *menu, int *file_id, int *file_descriptor)
{
    while (!*file_descriptor)
    {
        // Wait
    }
}

int change_screen(Screen *currentScreen, Screen *new_screen, int *index_pointer, int index, int with_refresh)
{
    if (index < 0)
    {
        printf("[ERROR] [BUG] THE SCREEN INDEX IS OUT OF RANGE\n");
        return 1;
    }

    currentScreen = new_screen;
    *index_pointer = index;

    if (with_refresh)
    {
        refresh(currentScreen, NULL);
    }
}

int handle_names()
{

    Screen *names = create_screen();
    ScreenComponent *titleComponent = create_screen_component(NULL);
    add_component(names, titleComponent);
    ScreenComponent *namesComponent = create_screen_component(names);
    ScreenComponent *footerComponent = create_screen_component(NULL);

    add_component(names, footerComponent);

    add_line(namesComponent, "\tNombres\n", GREEN);
    add_line(namesComponent, SUBSEPARATOR, GREEN);
    add_line(namesComponent, "\tNombre\tApellido\tCarnet", WHITE);
    add_line(namesComponent, SUBSEPARATOR, GREEN);
    add_line(namesComponent, "\n", WHITE);
    add_line(namesComponent, "\tAndrés\tQuan-Littow\t17652\n", WHITE);
    add_line(namesComponent, "\tPablo\tRuiz\t\tCarnet\n", WHITE);
    add_line(namesComponent, "\tMichael\tChan\t\tCarnet\n", WHITE);
    add_line(namesComponent, SEPARATOR, WHITE);
    add_line(namesComponent, "\t1. Atrás", WHITE);
    add_line(footerComponent, "\n\n====================\n>>> ", WHITE);

    refresh(names, NULL);
    return 0;
}