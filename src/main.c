#include "core/Screen.h"
#include "main.h"
#include "core/ConsoleController.h"
#include "core/ErrorHandling.h"
#include "core/Screen.h"

#include <stdlib.h>

#define SEPARATOR "\n=====================================\n"
#define SUBSEPARATOR "\n____________________________________\n"

int main(int argc, char *argv[])
{

    int file_loaded_message;
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

    add_line(titleComponent, "Proyecto 1");
    add_line(titleComponent, SEPARATOR);
    add_line(fileComponent, "Archivo: ");
    file_loaded_message = add_line(fileComponent, "NO CARGADO\n");
    add_line(fileComponent, SEPARATOR);
    add_line(bodyComponent, "\tMenu Principal\t");
    add_line(bodyComponent, SUBSEPARATOR);
    add_line(bodyComponent, "\n\tPor favor seleccione una opcion\n");
    add_line(bodyComponent, "\t1. Nombres\n\t2. Salir");
    add_line(namesComponent, "\tNombres\n\t_________________\n\n");
    add_line(namesComponent, "\tNombre\tApellido\tCarnet\n");
    add_line(namesComponent, "\tAndrés\tQuan-Littow\t17652\n");
    add_line(namesComponent, "\tPablo\tRuiz\t\tCarnet\n");
    add_line(namesComponent, "\tMichael\tChan\t\tCarnet\n");
    add_line(namesComponent, SEPARATOR);
    add_line(namesComponent, "\t1. Atrás");

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