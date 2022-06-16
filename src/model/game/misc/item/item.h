#ifndef ITEM
#define ITEM

#include <stdlib.h>
#include "../effect/effect.h"

typedef enum
{
    NONE_ITEM,
    LUNCH_BOX,
    WEAPON,
    HELMET,
    ARMOR,
    SHOES,
} ITEM_ENCYCLOPEDIA_ID;

typedef struct
{
    char *name;
    effect_t **launcherEffects;
    int launcherEffectsSize;
    effect_t **receiverEffects;
    int receiverEffectsSize;
    char *technic;
    char *description;
} item_t;

typedef struct
{
    char *name;
    int launcherEffects[6][2];
    int launcherEffectsSize;
    int receiverEffects[6][2];
    int receiverEffectsSize;
    char *technic;
    char *description;
} item_import;

item_t *createItem(
    char *name,
    int launcherEffectsSize,
    int launcherEffects[launcherEffectsSize][2],
    int receiverEffectsSize,
    int receiverEffects[receiverEffectsSize][2],
    char *technic,
    char *description);

item_t *importItem(item_import itemImport);
item_t *importItemFromId(ITEM_ENCYCLOPEDIA_ID itemId);
item_t **importEntityItemFromIdArray(int itemLength, int itemsId[itemLength]);

// DISPLAY FUNCTION

void displayItem(item_t *item);

// TEST FUNCTION

void testItem();

extern item_import ITEM_ENCYCLOPEDIA[];
#endif