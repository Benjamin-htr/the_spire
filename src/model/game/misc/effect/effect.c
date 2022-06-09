#include "effect.h"
#include <stdlib.h>
#include <stdio.h>

// GLOBAL VARIABLES

char *const EFFECT_NAME[10] = {
    "DAMAGE",
    "DODGE",
    "MANA",
    "ENERGY",
    "STRENGTH",
    "DEXTERITY",
    "FIRE",
    "WEAKNESS",
    "SLOWNESS",
    "sheee",
};

// CONSTRUCTORS

effect_t *initEffect(effect_ID id, int value)
{
    effect_t *res = malloc(sizeof(effect_t));
    res->id = id;
    res->value = value;
    return res;
}

// used in entity
effect_t *initEffectBar()
{
    effect_t *res = calloc(5, sizeof(effect_t *));
    for (size_t effects_ID = 0; effects_ID < 5; effects_ID++)
    {
        res[effects_ID] = *initEffect(effects_ID + 4, 0);
    };
    return res;
}

// effects : [[effectID, value]]
effect_t *initEffectFromArray(int effects[][2], size_t size)
{
    effect_t *res = calloc(size, sizeof(effect_t *));
    for (size_t effects_ID = 0; effects_ID < size; effects_ID++)
    {
        res[effects_ID] = *initEffect(effects[effects_ID][0], effects[effects_ID][1]);
    };
    return res;
}

// DISPLAY FUNCTION

void displayEffect(effect_t effect)
{
    printf(
        "\nnom: %s\nvaleur: %d\n",
        EFFECT_NAME[effect.id],
        effect.value);
}

// TEST FUNCTION
void testEffect()
{
    effect_t *test = initEffect(STR_E, 10);
    displayEffect(*test);
}