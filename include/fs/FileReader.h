#ifndef FILEREADER_PROJECT_H
#define FILEREADER_PROJECT_H

typedef struct StringVector
{
    char *name;
    void *value;
} StringVector;

char *read_file(char *path);
char **read_properties(char *properties_string);

#endif