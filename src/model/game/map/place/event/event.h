#ifndef EVENT
#define EVENT

#include "../../../../misc/select/option/option.h"

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
int do_nothing(entity_t *peter);
int launch_fight_miniboss(entity_t *peter);
int sanctuary_life_refill(entity_t *peter);
int sanctuary_show_deck();
int sanctuary_mana_refill(entity_t *peter);
int transform_striketododge(entity_t *peter);
int transform_dodgetostrike(entity_t *peter);
int mana_max_refill(entity_t *peter);
int life_max_refill(entity_t *peter);
int no_tp(entity_t *peter);
int tp(entity_t *peter);

extern event_import EVENT_ENCYCLOPEDIA[];

#endif