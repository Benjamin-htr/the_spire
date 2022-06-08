#include "event.h"
#include <stdlib.h>

event * createEvent(char * dialogue, select* actions, void * data){
    event * res = malloc(sizeof(event));
    res->dialogue = dialogue;
    res->actions=actions;
    res->data=data;
    return res;
};