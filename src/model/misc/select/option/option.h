#ifndef OPTION
#define OPTION

#include "../../../game/entity/entity.h"
#include "../../../game/combat/combat.h"

typedef int (*action_t)(entity_t *all_entity);

typedef struct
{
    char *label;
    action_t action;
} option;

option *option_init(char *label, action_t action);

#endif