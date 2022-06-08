#include "stat.h"
#include <stdlib.h>
#include <stdio.h>

#include "../../../misc/boolean/boolean.h"

const char *STAT_NAME[] = {
    "NIY",
    "VIE",
    "ESQUIVE",
    "FORCE",
    "MANA",
    "ENERGY",
};

stat_t *initStat(stat_ID id, int max, boolean currZero)
{
    stat_t *stat = malloc(sizeof(stat_t));
    stat->id = id;
    int **tmpStats;
    tmpStats = calloc(2, sizeof *tmpStats);
    for (int currOrMax = 0; currOrMax < 2; currOrMax++)
    {
        tmpStats[currOrMax] = calloc(2, sizeof *(tmpStats[currOrMax]));
        if (currOrMax || !currZero)
        {
            for (size_t temporalRange = 0; temporalRange < 2; temporalRange++)
            {
                tmpStats[currOrMax][temporalRange] = max;
            }
        }
    }
    stat->stat_bar = tmpStats;
    return stat;
}

void displayStat(stat_t stat)
{
    printf(
        "\n%s\nw/O Effect : \t%d/%d\nw Effect : \t%d/%d\n",
        STAT_NAME[stat.id],
        stat.stat_bar[CURR][PERCISTANT],
        stat.stat_bar[MAX][PERCISTANT],
        stat.stat_bar[CURR][TEMPORARY],
        stat.stat_bar[MAX][TEMPORARY]);
}

void testStat()
{
    stat_t *test = initStat(STR, 20, false);
    displayStat(*test);
}
