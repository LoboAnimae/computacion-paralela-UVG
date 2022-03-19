#include <stdlib.h>
#include <stdio.h>
#include "core/Screen.h"
#include "core/DataStructures.h"
#include "core/Colors.h"

const char *saved_colors[] = {
    "\033[0;30m",
    "\033[0;31m",
    "\033[0;32m",
    "\033[0;33m",
    "\033[0;34m",
    "\033[0;35m",
    "\033[0;36m",
    "\033[0;37m"};
int print_component_line(char *content, ProjectColors color)
{
    int color_print = printf("%s", saved_colors[color]);
    int content_print = printf("%s", content);
    int reset_print = printf("%s", saved_colors[WHITE]);
    return color_print && content_print && reset_print;
    // return 1;
}

/**
 * @brief Refreshes the screen with a set of controllers, printed one after another.
 *
 * @param screen The screen to be used to show in the console
 * @param result A result error handler
 * @return int -1 if there was an error, 0 otherwise
 */
int refresh(Screen *screen, Result *result)
{
    if (clear_screen(result) == -1)
    {
        return -1;
    }
    // For each component
    for (int i = 0; i < screen->size; i++)
    {
        ScreenComponent *first_component = screen->components[i];
        Line *current_line = first_component->lines[0];
        printf("Hello World!\n");
        int option;
        scanf("%d", &option);
        // Print every line and
        while (current_line != NULL)
        {
            // Unless it fails,
            if (print_component_line(current_line->content, current_line->color) == 0)
            {
                if (result != NULL)
                    result->message = "Printf returned an error";
                return -1;
            }
            // Move to the next line until done
            current_line = current_line->next;
        }
    }
    return 0;
}

/**
 * @brief Adds a component to a screen
 *
 * @param target The screen to add the component to
 * @param component The component to add
 * @return int -1 if there was an error, the id of the component otherwise
 */
int add_component(Screen *target, ScreenComponent *component)
{
    target->size++;
    target->components = (ScreenComponent **)realloc(target->components, sizeof(ScreenComponent *) * target->size);
    int newPosition = target->size - 1;
    component->id = newPosition;
    target->components[newPosition] = component;
    return newPosition;
}

int string_compare(char *firstString, char *secondString)
{
    int lengthOne = sizeof(firstString) / sizeof(char);
    int lengthTwo = sizeof(secondString) / sizeof(char);
    if (lengthOne != lengthTwo)
    {
        return -1;
    }

    for (int i = 0; i < lengthOne; i++)
    {
        if (firstString[i] != secondString[i])
        {
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Removes a component from a screen.
 *
 * @param target The screen to remove the component from.
 * @param id The id of the component to remove.
 * @param result A result component to store the error handling
 * @return int -1 if failure. The new size if successful.
 */
int remove_component(Screen *target, int id, Result *result)
{
    // Check if the id is valid
    if (target->size <= id)
    {
        if (result != NULL)
            result->message = "Component does not exist";
        return -1;
    }
    if (id < 0)
    {
        if (result != NULL)
            result->message = "Component id cannot be negative";
        return -1;
    }
    for (int i = 0; i < target->size; i++)
    {
        if (target->components[i]->id == id)
        {
            // Free the component
            free(target->components[i]);
            // Shift the array
            for (int j = i; j < target->size - 1; j++)
            {
                target->components[j] = target->components[j + 1];
            }
            // Decrement the size
            target->size--;
            // Reallocate the array
            target->components = (ScreenComponent **)realloc(target->components, sizeof(ScreenComponent *) * target->size);
            return target->size;
        }
    }
}

/**
 * @brief Adds a line to a component
 *
 * @param target The component to add the line to
 * @param line The line to add
 * @return int -1 if failure. The line id if success
 */
int add_line(ScreenComponent *target, char *line, ProjectColors color)
{
    target->size++;
    target->lines = (Line **)realloc(target->lines, sizeof(Line *) * target->size);
    int newPosition = target->size - 1;
    Line *newLine = (Line *)malloc(sizeof(Line));
    newLine->content = line;
    newLine->length = sizeof(line) / sizeof(char);
    newLine->id = newPosition;
    newLine->next = NULL;
    if (color != -1)
    {
        newLine->color = color;
    }
    else
    {
        newLine->color = WHITE;
    }
    target->lines[newPosition] = newLine;
    // Point the previous line to the new line
    if (newPosition > 0)
    {
        target->lines[newPosition - 1]->next = newLine;
    }
    return newPosition;
}

/**
 * @brief Removes a line using an ID
 *
 * @param target The ScreenComponent to remove the line from
 * @param id The id of the line
 * @param result A Result component for error handling
 * @return int -1 if not successful. The new size if it was.
 */
int remove_line(ScreenComponent *target, int id, Result *result)
{
    // Check if the id is valid
    if (target->size <= id)
    {
        if (result != NULL)
            result->message = "Line does not exist";
        return -1;
    }
    if (id < 0)
    {
        if (result != NULL)
            result->message = "Line id cannot be negative";
        return -1;
    }
    if (id == 0)
    {
        // Get the first line
        Line *first_line = target->lines[0];
        // Move the pointer to the next line
        target->lines[0] = first_line->next;
        // Free the first line
        free(first_line);
        target->size--;
        target->lines = (Line **)realloc(target->lines, sizeof(Line *) * target->size);
        return target->size;
    }
    else if (id == target->size - 1)
    {
        // Get the last line
        Line *last_line = target->lines[id];
        // Move the pointer to the previous line
        target->lines[id - 1]->next = NULL;
        // Free the last line
        free(last_line);
        target->size--;
        target->lines = (Line **)realloc(target->lines, sizeof(Line *) * target->size);
        return target->size;
    }
    else
    {
        // Get the line to remove
        Line *line_to_remove = target->lines[id];
        // Move the pointer to the previous line
        target->lines[id - 1]->next = line_to_remove->next;
        // Free the line
        free(line_to_remove);
        target->size--;
        target->lines = (Line **)realloc(target->lines, sizeof(Line *) * target->size);
        return target->size;
    }
    Line *previous = target->lines[id - 1]->next;
    Line *current = target->lines[id];
    // Line *next = current->next;
    // previous->next = next;
    printf("%d->(del)%d->%d\n%d->%d\n", previous->id, current->id, previous->id, previous->id, previous->id);
    for (int i = 0; i < 1000; i++)
    {
        // Do nothing
    }
    free(current);
    return 0;
}

int update_line(ScreenComponent *target, int id, char *line, ProjectColors new_color, Result *result)
{
    // Check if the id is valid
    if (target->size <= id)
    {
        if (result != NULL)
            result->message = "Line does not exist";
        return -1;
    }
    if (id < 0)
    {
        if (result != NULL)
            result->message = "Line id cannot be negative";
        return -1;
    }
    // Update the content of the line
    Line *toBeUpdated = target->lines[id];
    toBeUpdated->color = new_color;
    toBeUpdated->content = line;
    toBeUpdated->length = sizeof(line) / sizeof(char);
    return toBeUpdated->id;
}

/**
 * @brief Clears the screen
 *
 * @param controller A result controller that allows for error handling.
 */
int clear_screen(Result *controller)
{
#ifdef _WIN32
    if (system("cls") == -1 && controller != NULL)
    {
        controller->message = "Error clearing screen";
        return -1;
    }

#else
    if (system("clear") == -1 && controller != NULL)
    {
        controller->message = "Error clearing screen";
        return -1;
    }
#endif
    return 0;
}

Screen *create_screen()
{
    // Before allocating, find out if the screen exists
    Screen *screen = (Screen *)malloc(sizeof(Screen));
    screen->size = 0;
    screen->components = NULL;
    return screen;
}

ScreenComponent *create_screen_component(Screen *parent)
{
    ScreenComponent *component = (ScreenComponent *)malloc(sizeof(ScreenComponent));
    component->id = 0;
    component->size = 0;
    component->lines = NULL;
    if (parent != NULL)
    {
        add_component(parent, component);
    }
    return component;
}

/**
 *
 * @param target
 * @param result
 * @return int
 */
int flush_component(ScreenComponent *target, Result *result)
{
    for (int i = 0; i < target->size; i++)
    {
        remove_line(target, i, NULL);
    }
    target->size = 0;
    target->lines = NULL;
    return 0;
}