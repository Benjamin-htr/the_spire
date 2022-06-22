#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "option.h"

// init d'une option
option *option_init(char *label, action_t action)
{
    option *o = malloc(sizeof(option));
    o->label = label;
    o->action = action;
    return o;
}

// retourne les actions de l'option
action_t option_get_action(option *o)
{
    return o->action;
}

action_t action_init()
{
    return NULL;
}
