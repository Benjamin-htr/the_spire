#ifndef EFFECT
#define EFFECT

typedef enum
{
    HP_E,
    DODGE_E,
    ABISS_E,
    STR_E,
    DEX_E,
    FIRE_E,
    WEAK_E,
    SLOW_E,
} effect_ID;

typedef struct
{
    effect_ID id;
    int value;
} effect_t;

effect_t *initEffect(effect_ID id, int value);
void displayEffect(effect_t effect);
void testEffect();

#endif