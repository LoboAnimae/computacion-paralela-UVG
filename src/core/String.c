#include "core/String.h"
#include <stdlib.h>
#include <stdio.h>

String *concat(char *s1, char *s2)
{
    int length_s1 = sizeof(s1) / sizeof(char);
    int length_s2 = sizeof(s2) / sizeof(char);
    int totalLength = length_s1 + length_s2 + 1;
    char *content = (char *)malloc(sizeof(char) * totalLength);

    for (int i = 0; i < totalLength; ++i)
    {
        if (i < length_s1)
        {
            content[i] = s1[i];
        }
        else
        {
            content[i] = s2[i - length_s1];
        }
    }
    content[totalLength - 1] = '\0';
    String *new_string = (String *)malloc(sizeof(String));
    new_string->length = totalLength;
    new_string->content = content;
    return new_string;
}

int get_string_length(char *s)
{
    int length = 0;
    while (s[length] != '\0')
    {
        ++length;
    }
    return length;
}

String *from_char_ptr(char *s)
{
    int length = get_string_length(s);
    char *content = (char *)malloc(sizeof(char) * length);
    for (int i = 0; i < length; ++i)
    {
        content[i] = s[i];
    }
    content[length] = '\0';
    String *new_string = (String *)malloc(sizeof(String));
    new_string->length = length;
    new_string->content = content;
    return new_string;
}

Params *load_params(char *file_name, char *delimiter)
{
    FILE *input = fopen(file_name, "r");
    if (input == NULL)
    {
        printf("Error opening file %s\n", file_name);
        return NULL;
    }
    // Read the file
    fseek(input, 0, SEEK_END);
    int fileLength = ftell(input);
    fseek(input, 0, SEEK_SET);
    char *input_string = (char *)malloc(sizeof(char) * fileLength);
    fread(input_string, sizeof(char), fileLength, input);
    fclose(input);
}