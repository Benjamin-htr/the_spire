#ifndef GLOBAL
#define GLOBAL

#include "card.h"
#include "stddef.h"

card_t STRIKE = {
    .name = "Strike",
    .rarity = BASIC,
    .description = "Attaque de base",
    .energyCost = 1,
    .manaCost = 0,
    .technic = "Inflige 6 dégâts",
};

card_t DODGE_A = {
    .name = "Dodge",
    .rarity = BASIC,
    .description = "L'esquive de base",
    .energyCost = 1,
    .manaCost = 0,
    .technic = "Donne 5 points d'equive",
};

card_t DOUBLE_STRIKE = {
    .name = "Double strike",
    .rarity = COMMON,
    .description = "Attaque de base",
    .energyCost = 1,
    .manaCost = 0,
    .technic = "Inflige 6 dégâts",
};

card_t FIREBALL = {
    .name = "Boule de feu",
    .rarity = COMMON,
    .description = "Un classique des mages débutants",
    .energyCost = 0,
    .manaCost = 20,
    .technic = "Inflige 6 points de feu",
};

card_t WEAKENING_STRIKE = {
    .name = "Coup affaiblissant",
    .rarity = COMMON,
    .description = "Permet d'attaquer tout en se défendant",
    .energyCost = 2,
    .manaCost = 0,
    .technic = "Inflige 3 dégâts et rends l'ennemi faible pour 2 tours",
};

card_t ACCELERATION = {
    .name = "acceleration",
    .rarity = COMMON,
    .description = "Un classique des mages débutants",
    .energyCost = 1,
    .manaCost = 10,
    .technic = "Inflige Lent pour un tour et donne 4 points d'esquive",
};

card_t OVERWORK = {
    .name = "Surménage",
    .rarity = ATYPIC,
    .description = "Permet de frpper plus fort, mais attention aux courbatures!",
    .energyCost = 1,
    .manaCost = 20,
    .technic = "Augmente les dégâts de 2, mais vous inflige 5 dégâts",
};

card_t DEFENSE = {
    .name = "Posture défensive",
    .rarity = ATYPIC,
    .description = " Permet de mieux esquiver, mais faire durer les combats plus longtemps",
    .energyCost = 0,
    .manaCost = 0,
    .technic = "Dextérité +2, mais Force -1",
};

card_t FOCUS = {
    .name = "Concentration",
    .rarity = ATYPIC,
    .description = ": Important si les combats durent longtemps",
    .energyCost = 3,
    .manaCost = 0,
    .technic = "Recharge 50 points de mana",
};

card_t FIRE = {
    .name = "Incendie",
    .rarity = ATYPIC,
    .description = "Aidera à terminer les combats, d’une manière ou l’autre",
    .energyCost = 2,
    .manaCost = 20,
    .technic = "Inflige 10 de feu à l’adversaire et 5 de feu à soi",
};

card_t PULVERIZE = {
    .name = "Pulvériser",
    .rarity = RARE,
    .description = "Blam ",
    .energyCost = 3,
    .manaCost = 0,
    .technic = "Inflige 30 dégâts, Abyssal",
};

card_t SPECTRUM = {
    .name = "Spectre complet",
    .rarity = RARE,
    .description = "Un bonus pour chaque couleur de l’arc-en-ciel.",
    .energyCost = 2,
    .manaCost = 20,
    .technic = "Donne différents bonus. Abyssal.",
};

card_t JAWURM_BACKSTAB = {
    // 11 dégats
    .name = "Tape dans le dos",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Coup de pute",
};

card_t JAWURM_FIST = {
    // 7 dégats
    .name = "Coup de poing",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Un autre classique",
};

card_t JAWURM_CROUCH = {
    // force+3, 6 points d'esquive
    .name = "Accroupissement",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Rien de mieux pour reprendre son élan",
};

card_t BLOUNI_KICK = {
    // 13 dégâts
    .name = "Coup de pied",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Bien placé, il peut faire très mal",
};

card_t BLOUNI_JAB = {
    // 8 dégâts
    .name = "Jab",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Un classique",
};

card_t KELIKO_NUDGE = {
    // 3 dégâts, 3 points d'esquive,force +2
    .name = "Coup de coude",
    .energyCost = 0,
    .manaCost = 0,
    .description = "De quoi refaire sa dentition..",
};

card_t KELIKO_PINCH = {
    // : 3 dégâts, 3 points d’esquive, dextérité +2
    .name = "Pincement",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Bah ?",
};

card_t HEADBUTT = {
    // 15 dégats
    .name = "Coup de boule",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Aie mon crâne",
};

card_t JAWURM2_HAIRPULLING = {
    // 12 dégats,10 points d'esquive
    .name = "Tirage de cheveux",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Un pas de plus pour la calvitie..",
};

card_t JAWURM2_SPIT = {
    // +3 force, 18 points d'esquive
    .name = "Crachat",
    .energyCost = 0,
    .manaCost = 0,
    .description = "On te manque de respect",
};

card_t REDONI_SPANKING = {
    // 20 dégâts
    .name = "fessée",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Tu n'as pas été sage..",
};

card_t REDONI_SLAP = {
    // 15 degâts, 2 tours de faiblesse sur Peter
    .name = "Gifle",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Clac",
};

card_t MANGOUSTINE_SNARE = {
    //  3 dégâts, 3 points d’esquive, force +2 sur lui, dextérité +2 sur lui (
    .name = "Ricannement",
    .energyCost = 0,
    .manaCost = 0,
    .description = "On se moque de toi..",
};

card_t ELDAN_BANANAPEEL = {
    // : 10 dégâts, 15 points d’esquive
    .name = "Peau de banane",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Attention à la peau de banane",
};

card_t PYROX_FIRESPIT = {
    // Feu 5, 10 points d’esquive
    .name = "Crachat de feu",
    .energyCost = 0,
    .manaCost = 0,
    .description = "On te crache du feu dessus",
};

card_t PYROX_FIRESTORM = {
    // Feu 10, dextérité +2 (bonus de dextérité pour lui)
    .name = "Tempête de feu",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Une tempête de feu s'abat sur toi",
};

card_t SLEEP = {
    .name = "Dors..",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Le boss s'endort",
};

card_t CLAW_COMBO = {
    //: Dégâts (4), Dégâts (4), Dégâts (4), Dégâts (4), Dégâts (4)
    .name = "Combo griffe",
    .energyCost = 0,
    .manaCost = 0,
    .description = "Le boss te rafale ta mère",
};

#endif