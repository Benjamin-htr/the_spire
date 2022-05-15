#ifndef OPTION
#define OPTION

typedef void*(*action_t)();

typedef struct
{
    char* label;
    action_t action;
}option;


#endif