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
    effect_t *launcherEffects;
    size_t launcherEffectsSize;
    effect_t *receiverEffects;
    size_t receiverEffectsSize;
    char *technic;
    char *description;
} item_t;

typedef struct
{
    char *name;
    int launcherEffects[6][2];
    size_t launcherEffectsSize;
    int receiverEffects[6][2];
    size_t receiverEffectsSize;
    char *technic;
    char *description;
} item_import;

item_t *createItem(
    char *name,
    int launcherEffects[6][2],
    size_t launcherEffectsSize,
    int receiverEffects[6][2],
    size_t receiverEffectsSize,
    char *technic,
    char *description);

item_t *importItem(item_import itemImport);
item_t *importItemFromId(ITEM_ENCYCLOPEDIA_ID itemId);
item_t *importItemFromIdArray(int itemsId[], int itemLength);

// DISPLAY FUNCTION

void displayItem(item_t item);

// TEST FUNCTION

void testItem();

extern item_import ITEM_ENCYCLOPEDIA[];
#endif