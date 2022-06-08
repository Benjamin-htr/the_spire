#include "effect.h"
#include <stdlib.h>
#include <stdio.h>

const char *EFFECT_NAME[8] = (const char *[]){
    "DAMAGE",
    "DODGE",
    "ABISS",
    "STRENGTH",
    "DEXTERITY",
    "FIRE",
    "WEAKNESS",
    "SLOWNESS",
};

effect_t *
initEffect(effect_ID id, int value)
{
    effect_t *res = malloc(sizeof(effect_t));
    res->id = id;
    res->value = value;
}
void displayEffect(effect_t effect)
{
    printf(
        "\nnom: %s\nvaleur: %d\n",
        EFFECT_NAME[effect.id],
        effect.value);
}
void testEffect()
{
    effect_t *test = initEffect(STR_E, 10);
    displayEffect(*test);
}