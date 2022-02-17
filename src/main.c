#include "main.h"
#include "core/ConsoleController.h"
#include "core/ErrorHandling.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Line *controller = create_line_controller();
    controller->add_line(controller, "Project 1");
    controller->add_line(controller, "===============\nPlease choose one:");
    controller->add_line(controller, "\t1. Group information");
    controller->add_line(controller, "\t2. Exit");
    refresh(controller);
    free(controller);
    refresh(controller2);
    return 0;
}