#ifndef STAT
#define STAT

#include "../../../misc/boolean/boolean.h"

typedef enum
{
    NONE,
    HP,
    DODGE,
    MANA,
    ENERGY,
} stat_ID;

typedef enum
{
    PERCISTANT,
    TEMPORARY,
} statTemporalRange;

typedef enum
{
    CURR,
    MAX,
} statCurrOrMax;

typedef struct
{
    stat_ID id;
    int **stat_bar;
} stat_t;

// CONSTRUCTOR
stat_t *initStat(stat_ID id, int max, boolean currZero); // If curr is null curr = max
stat_t *initEntityStatFromArray(int stats[][2]);

// DISPLAY FUNCTION
void displayStat(stat_t stat);

// METHOD
void updateStat(stat_t *stat, int value, statCurrOrMax currOrMax, statTemporalRange temporalRange);

// TEST FUNCTION
void testStat();

#endif