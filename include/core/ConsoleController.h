#ifndef CONSOLE_CONTROLLER_H
#define CONSOLE_CONTROLLER_H
#include <stdio.h>
#include <core/ErrorHandling.h>
#include <core/Screen.h>
// typedef struct Line
// {
//     char *content;
//     int id;
//     int length;
//     struct Line *next;
//     int (*add_line)(struct Line *, char *);
// } Line;

// int refresh(Line **lines);
Line *create_line_controller();
#endif