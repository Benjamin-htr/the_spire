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

typedef struct
{
    stat_ID id;
    int current;
    int max;
} stat_t;

// CONSTRUCTOR
stat_t *initStat(stat_ID id, int max, boolean currZero); // If curr is null curr = max
stat_t **initEntityStatFromArray(int stats[4][2]);

// DECONSTRUCTOR
void freeStat(stat_t *statPt);
void freeEntityStatArray(stat_t **entityStatArray);

// DISPLAY FUNCTION
void displayStat(stat_t *stat);
void displayEntityStatArray(stat_t **entityStatArray);

// METHOD
void updateStat(stat_t *stat, int value, boolean isMax);
void refillStat(stat_t *stat);

// TEST FUNCTION
void testStat();

#endif