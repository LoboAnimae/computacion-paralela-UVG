#include "core/IO.h"
#include <stdio.h>
#include <stdlib.h>

int save_to_file(int *array, int size, char *filename, char separator)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    for (int i = 0; i < size; i++)
    {
        if (i == size - 1)
        {
            fprintf(file, "%d", array[i]);
        }
        else
        {
            fprintf(file, "%d%c", array[i], separator);
        }
    }
    printf("Wrote to file %s\n", filename);
    fclose(file);
    return 0;
}

char *read_from_file(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening file!\n");
        return NULL;
    }
    fseek(file, 0, SEEK_END);

    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("Reading file %s\n", filename);
    // Read the file
    char *buffer = malloc(sizeof(char) * size);
    fread(buffer, 1, size, file);
    fclose(file);
    return buffer;
}

int *split(char *str, char separator, int length)
{
    int pointer = 0;
    int *numbers = malloc(length * sizeof(int));
    int numberPointer = 0;
    int i = 0;
    int stringPointer = 0;
    char *currentNumber = malloc(sizeof(char) * 100);
    while (str[i] != '\0')
    {
        // If the string is a separator
        if (str[i] == separator)
        {
            stringPointer = 0;
            i++;
            continue;
        }
        // As soon as a number is found, start copying it
        while (str[i] != separator && str[i] != '\0')
        {
            currentNumber[stringPointer] = str[i];
            stringPointer++;
            i++;
        }
        numbers[numberPointer] = atoi(currentNumber);
        for (int j = 0; j < 100; ++j)
        {
            currentNumber[j] = ' ';
        }
        numberPointer++;
    }
    return numbers;
}
