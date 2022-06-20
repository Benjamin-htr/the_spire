#ifndef EVENT
#define EVENT

#include "../../../../misc/select/select.h"

typedef struct
{
    char *dialogue;
    option **actions;
    void *data;
} event;

typedef struct
{
    char *dialogue;
    void *data;
    option actions[2];
} event_import;

event *event_init(char *dialogue, option *actions, void *data);
event *importEvent(event_import event_import_);
void Test_EVENT();
event *get_sanctuary();
event *get_mini_boss();
event **import_all_event();
event *get_random_event();
void *do_nothing();

extern event_import EVENT_ENCYCLOPEDIA[];

#endif