#ifndef CONSOLE_CONTROLLER_H
#define CONSOLE_CONTROLLER_H
#include <stdio.h>
#include <core/ErrorHandling.h>
typedef struct Line
{
    char *content;
    int id;
    int length;
    struct Line *next;
    int (*add_line)(struct Line *, char *);
} Line;

void clear_screen(Result *controller);
void set_screen(char *lines, Result *controller);
int refresh(Line *controllers);
int add_line(Line *controller, char *newLine);
Line *create_line_controller();
#endif