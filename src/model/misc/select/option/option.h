#ifndef OPTION
#define OPTION

typedef void*(*action_t)();

typedef struct
{
    char* label;
    action_t action;
}option;


option option_init(char* label, action_t action);

#endif