#ifndef SCREEN_IMPL_H
#define SCREEN_IMPL_H
#include "ErrorHandling.h"
#include "DataStructures.h"
#include "Colors.h"
typedef struct ScreenComponent ScreenComponent;
typedef struct Line Line;
typedef struct Screen Screen;
typedef struct ScreenController ScreenController;

struct Screen
{
    ScreenComponent **components;
    int size;
};

struct ScreenController
{
    Screen *current_screen;
    int screen_index;
    LinkedList *screen_vector;
};

/**
 * Components are objects that can be drawn on the screen. They are simple and composed of Lines.
 * @brief is a component on the screen. It can be easily updated without updating all the other components.
 */
struct ScreenComponent
{
    Line **lines;
    int size;
    int id;
};

/**
 * @brief A line is a part of a component. It represents text and characters inside of it. It works as a doubly linked list.
 */
struct Line
{
    char *content;
    int length;
    int id;
    ProjectColors color;
    struct Line *next;
};

int refresh(Screen *screen, Result *result);

int add_component(Screen *target, ScreenComponent *component);
int remove_component(Screen *target, int id, Result *result);

int add_line(ScreenComponent *target, char *newLine, ProjectColors color);
int remove_line(ScreenComponent *target, int id, Result *result);
int update_line(ScreenComponent *target, int id, char *newLine, ProjectColors new_color, Result *result);
int clear_screen(Result *controller);
int print_line(Line *line, Result *result);
int flush_component(ScreenComponent *target, Result *result);
int print_component_line(char *content, ProjectColors color);

Screen *create_screen();
ScreenComponent *create_screen_component(Screen *parent);

// Create _Generic for clear_screen
#endif