#include <stdlib.h>
#include <string.h>
#include "event.h"

event *event_init(char* dialogue, select_* actions, void* data)
{
    event* e = malloc(sizeof(event));
    e->dialogue = dialogue;
    e->actions = actions;
    e->data = data;
    return e;
}

