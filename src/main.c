#include "core/Screen.h"
#include "main.h"
#include "core/ConsoleController.h"
#include "core/ErrorHandling.h"
#include "core/Screen.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{

    // Create a screen
    Screen *screen = create_screen();
    ScreenComponent *titleComponent = create_screen_component();
    ScreenComponent *bodyComponent = create_screen_component();
    ScreenComponent *footerComponent = create_screen_component();
    add_component(screen, titleComponent);
    add_component(screen, bodyComponent);
    add_component(screen, footerComponent);
    add_line(titleComponent, "Proyecto 1\n===================\n\n");
    add_line(bodyComponent, "Body component - ");
    int toBeUpdated = add_line(bodyComponent, "I will change on the 300th iteration");
    add_line(footerComponent, "\n\n===================\nThis is a footer component");
    refresh(screen, NULL);
    for (int i = 0; i < 1000; i++)
    {
        printf("\n\n\nDebug: Current Iteration->%d\n", i);
        if (i == 250)
        {
            toBeUpdated = update_line(bodyComponent, toBeUpdated, "I changed!", NULL);
        }
        else if (i == 400)
        {
            toBeUpdated = update_line(bodyComponent, toBeUpdated, "I changed again!", NULL);
        }
        else if (i == 600)
        {
            remove_line(bodyComponent, toBeUpdated, NULL);
        }
        refresh(screen, NULL);
    }
    return 0;
}