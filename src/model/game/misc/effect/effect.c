#include "effect.h"
#include <stdlib.h>
#include <stdio.h>

// GLOBAL VARIABLES

char *const EFFECT_NAME[13] = {
    "VIE",
    "ESQUIVE",
    "MANA",
    "ENERGIE",
    "VIE MAX",
    "ESQUIVE MAX",
    "MANA MAX",
    "ENERGIE MAX",
    "FORCE",
    "DEXTERITE",
    "FEU",
    "FAIBLESSE",
    "LENTEUR",
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
     printf("init effect bar \n");
    fflush(stdout);
    effect_t *res = malloc(sizeof(effect_t *));
    printf("calloc init effect bar \n");
    fflush(stdout);
    for (size_t effects_ID = 0; effects_ID < 9; effects_ID++)
    {
            printf("before init effet call\n");
    fflush(stdout);
        res[effects_ID] = *initEffect(effects_ID + 4, 0);
            printf("after init effectcall \n");
    fflush(stdout);
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