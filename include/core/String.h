#ifndef STRING_PROJECT_H
#define STRING_PROJECT_H

typedef struct String String;
typedef struct Params Params;
struct String
{
    int length;
    char *content;
};

struct Params
{
    String name;
    String content;
    String *next;
};

String *concat(char *s1, char *s2);
String *from_char_ptr(char *s);
Params *load_params(char *file_name, char *delimiter);
#endif