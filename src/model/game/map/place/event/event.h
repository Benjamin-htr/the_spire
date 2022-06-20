#ifndef EVENT
#define EVENT

// #include "select.h"
#include "../../../../misc/select/option/option.h"
// #include "../../map.h"

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
void *do_nothing(entity_t *peter, ...);
void *launch_fight_miniboss(entity_t *peter, ...);
void *sanctuary_life_refill(entity_t *peter, ...);
void *sanctuary_mana_refill(entity_t *peter, ...);
void *transform_striketododge(entity_t *peter, ...);
void *transform_dodgetostrike(entity_t *peter, ...);
void *mana_max_refill(entity_t *peter, ...);
void *life_max_refill(entity_t *peter, ...);
void *no_tp(entity_t *peter, ...);
void *tp(entity_t *peter, ...);

extern event_import EVENT_ENCYCLOPEDIA[];

#endif