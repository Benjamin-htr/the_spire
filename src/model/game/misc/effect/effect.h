#ifndef EFFECT
#define EFFECT

#include "../stat/stat.h"
#include "../../../misc/boolean/boolean.h"

typedef void (*onDisplay_t) ();
typedef void (*onNextTurn_t) ();
typedef void (*onCardUsed_t) ();
typedef void (*onCombatStart_t) ();


typedef struct _effect
{
    _effect (*init) (int);
    onDisplay_t onDisplay;
    onNextTurn_t onNextTurn;
    onCardUsed_t onCardUse_t;
    onCombatStart_t onCombatStart;
    statTemporalRange effectPercistance;
    boolean effectMax;
    int value;
    int turnLeft;
    char* description;
}effect;


#endif