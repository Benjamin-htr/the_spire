#ifndef ITEM
#define ITEM

#include <stdlib.h>
#include "../../effect/effect.h"
#include "../../../../misc/boolean/boolean.h"

enum rarityE
{
    COMMON,
    BASIC,
    ATYPIC,
    RARE,
};

typedef struct
{
    char *name;
    enum rarityE rarity;
    int manaCost;
    int energyCost;
    boolean isAbyssal;
    effect_t *launcherEffects;
    size_t launcherEffectsSize;
    effect_t *receiverEffects;
    size_t receiverEffectsSize;
    char *technic;
    char *description;
} card_t;

// CONSTRUCTOR

card_t *createCard(
    char *name,
    enum rarityE rarity,
    int manaCost,
    int energyCost,
    boolean isAbyssal,
    int launcherEffects[][2],
    size_t launcherEffectsSize,
    int receiverEffects[][2],
    size_t receiverEffectsSize,
    char *technic,
    char *description);

// DISPLAY FUNCTION

void displayCard(card_t card);

// TEST FUNCTION

void testCard();

#endif