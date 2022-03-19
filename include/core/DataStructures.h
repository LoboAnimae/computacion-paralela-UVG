#ifndef DATA_STRUCTURES_PROJECT_H
#define DATA_STRUCTURES_PROJECT_H

typedef struct LinkedList LinkedList;
typedef struct DoubleLinkedList DoubleLinkedList;
typedef struct Queue Queue;
typedef struct Stack Stack;

struct LinkedList
{
    void *data;
    LinkedList *next;
};

struct DoubleLinkedList
{
    void *data;
    DoubleLinkedList *previous;
    DoubleLinkedList *next;
};

struct Queue
{
    LinkedList *first;
    LinkedList *last;
};

struct Stack
{
    LinkedList *first;
};

void *add(LinkedList *list, void *data);

#endif