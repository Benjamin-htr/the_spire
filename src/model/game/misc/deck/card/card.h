#ifndef CARD
#define CARD

#include <stdlib.h>
#include "../../effect/effect.h"
#include "../../../../misc/boolean/boolean.h"

typedef enum
{
    STRIKE,
    DODGE_A,
    DOUBLE_STRIKE,
    FIREBALL,
    WEAKENING_STRIKE,
    ACCELERATION,
    OVERWORK,
    DEFENSE,
    FOCUS,
    FIRE,
    PULVERIZE,
    SPECTRUM,
    JAWURM_BACKSTAB,
    JAWURM_FIST,
    JAWURM_CROUCH,
    BLOUNI_KICK,
    BLOUNI_JAB,
    KELIKO_NUDGE,
    KELIKO_PINCH,
    HEADBUTT,
    JAWURM2_HAIRPULLING,
    JAWURM2_SPIT,
    REDONI_SPANKING,
    REDONI_SLAP,
    MANGOUSTINE_SNARE,
    ELDAN_BANANAPEEL,
    PYROX_FIRESPIT,
    PYROX_FIRESTORM,
    SLEEP,
    CLAW_COMBO,
} CARD_ENCYCLOPEDIA_ID;

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

typedef struct
{
    char *name;
    enum rarityE rarity;
    int manaCost;
    int energyCost;
    boolean isAbyssal;
    int launcherEffects[6][2];
    size_t launcherEffectsSize;
    int receiverEffects[6][2];
    size_t receiverEffectsSize;
    char *technic;
    char *description;
} card_import;
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

card_t *importCard(card_import cardImport);
card_t *importCardFromId(CARD_ENCYCLOPEDIA_ID cardId);

// DISPLAY FUNCTION

void displayCard(card_t card);

// TEST FUNCTION

void testCard();

extern card_import CARD_ENCYCLOPEDIA[];
#endif