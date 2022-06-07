#ifndef STAT
#define STAT

enum statID{
    NONE,
    HP,
    DODGE,
    STR,
    MANA,
    ENERGY,
};

typedef enum {
    PERCISTANT,
    COMBAT,
    TURN,
}statTemporalRange;

typedef enum {
    CURR,
    MAX,
}statTemporalRange_;

typedef int stat_value[3];
typedef stat_value stat_bar_t[2];
typedef struct
{
    enum statID id;
    stat_bar_t *stat_bar;
} stat;

#endif