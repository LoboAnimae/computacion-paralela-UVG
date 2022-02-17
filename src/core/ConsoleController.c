#include <stdlib.h>
#include <core/ConsoleController.h>

/*
 * clear_screen. Returns 1 if successful, 0 otherwise.
 * @desc: Clears the current terminal
 * @param: void
 * @return: int
 */
void clear_screen(Result *controller)
{
    int res;
#ifdef _WIN32
    if (system("cls") == -1)
    {
        controller->result = -1;
        controller->message = "Error clearing screen";
        return;
    }
#else
    if (system("clear") == -1)
    {
        controller->result = -1;
        controller->message = "Error clearing screen";
        return;
    }
#endif
}

/*
 * Sets the new screen. Returns 1 if successful, 0 otherwise.
 * @desc: Deploys a new screen.
 * @param: void
 * @return: int
 */
void set_screen(char *lines, Result *controller)
{
    if (printf("%s\n", lines) < 0)
    {
        controller->result = -1;
        controller->message = "Printf returned an error";
        return;
    }
}

/*
 * refresh. Returns 1 if successful, 0 otherwise.
 * @desc: Refreshes the screen with a set of controllers, printed one after another.
 * @param: struct ConsoleController *controllers
 * @return: int
 */
int refresh(Line *lines)
{
    Result *controller = (Result *)malloc(sizeof(Result));
    // Clear the screen
    clear_screen(controller);
    if (controller->result == -1)
    {
        printf("[ERROR] THERE WAS AN EXCEPTION IN THE REFRESH COMPONENT: %s\n", controller->message);
        free(controller);
        return -1;
    }

    Line *currentLine = lines;
    while (currentLine->next != NULL)
    {
        set_screen(currentLine->content, controller);
        currentLine = currentLine->next;
    }
    set_screen(currentLine->content, controller);
    free(controller);
    return 0;
}

int add_line(Line *controller, char *newLine)
{
    Line *currentLine = controller;
    while (currentLine->next != NULL)
    {
        currentLine = currentLine->next;
    }
    Line *newLineObject = (Line *)malloc(sizeof(Line));
    newLineObject->content = newLine;
    newLineObject->id = currentLine->id + 1;
    newLineObject->length = sizeof(newLine) / sizeof(char);
    newLineObject->next = NULL;
    currentLine->next = newLineObject;
    return 0;
}

Line *create_line_controller()
{
    Line *controller = (Line *)malloc(sizeof(Line));
    controller->content = "";
    controller->id = 0;
    controller->length = 0;
    controller->next = NULL;
    controller->add_line = &add_line;
    return controller;
}