#ifndef IO_PR_H
#define IO_PR_H

int save_to_file(int *array, int size, char *filename, char separator);

char *read_from_file(char *filename);
int *split(char *str, char separator, int size);
int *parse(char **strings, int size);
#endif