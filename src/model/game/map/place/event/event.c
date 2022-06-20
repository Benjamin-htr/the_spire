#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "event.h"

int NB_EVENT = 5;

event *event_init(char *dialogue, option *actions, void *data)
{
    event *e = malloc(sizeof(event));
    e->dialogue = dialogue;
    if (actions)
    {
        e->actions = calloc(2, sizeof(option));
        e->actions[0] = option_init(actions[0].label, actions[0].action);
        e->actions[1] = option_init(actions[1].label, actions[1].action);
    }
    else
    {
        e->actions = NULL;
    }
    e->data = data; // mettre un sanctuaire ici
    return e;
}
event *get_random_event()
{
    return importEvent(EVENT_ENCYCLOPEDIA[((rand() % (NB_EVENT - 1)) + 2)]);
}

event **import_all_event()
{
    event **all = malloc(sizeof(event) * NB_EVENT);
    for (int i = 0; i < NB_EVENT; i++)
    {
        all[i] = importEvent(EVENT_ENCYCLOPEDIA[i]);
    }
    return all;
}

void Test_EVENT()
{
    option actions[2] = {
        {
            .label = "OUI",
            .action = NULL,
        },
        {
            .label = "NON",
            .action = NULL,
        },
    };
    event *re = event_init("teststestesgqg", actions, NULL);
    printf("re.dialogue = %s\n", re->dialogue);
    printf("re.action 0 = %s\n", re->actions[0]->label);
    printf("re.action 1 = %s\n", re->actions[1]->label);

    printf("oouho\n");
    event *test = get_random_event();
    printf("oouho\n");
    printf("dialogue = %s\n", test->dialogue);
    event **po = import_all_event();
    printf("oouho\n");
    for (int i = 0; i < NB_EVENT; i++)
    {
        printf("dialogue = %s\n", po[i]->dialogue);
        printf("action 1 = %s\naction 2 = %s\n", po[i]->actions[0]->label, po[i]->actions[1]->label);
    }
    po[0]->actions[0]->action();
}

void show_event(event *ev)
{
    printf("ev.dialogue = %s\n", ev->dialogue);
    printf("ev.action 0 = %s\n", ev->actions[0]->label);
    printf("ev.action 1 = %s\n", ev->actions[1]->label);
}

event *importEvent(event_import event_import_)
{
    return event_init(event_import_.dialogue, event_import_.actions, event_import_.data);
}

event *get_mini_boss()
{
    return importEvent(EVENT_ENCYCLOPEDIA[0]);
}

event *get_sanctuary()
{
    return importEvent(EVENT_ENCYCLOPEDIA[1]);
}

void *do_nothing()
{
    printf("do nothing\n");
    return NULL;
}

event_import EVENT_ENCYCLOPEDIA[] = {

    {
        .dialogue = "Peter décourvre un miniboss entrain de dormir",
        .actions = {
            {
                .label = "Lancer un combat contre un miniboss (gains normaux en cas de victoire)",
                .action = &do_nothing,
            },
            {
                .label = "Ne pas faire le combat et avancer normalement",
                .action = &do_nothing,
            },
        },
        .data = NULL, // get random miniboss if event is miniboss
    },
    {
        .dialogue = "Vous êtes entré sans un sanctuaire !!",
        .actions = {
            {
                .label = "Dormir pour regagner la moitié de ses HP max",
                .action = &do_nothing,
            },
            {
                .label = "Méditer pour retirer une carte du deck principal (afin d'avoir de meilleurs chances de piocher les cartes plus fortes)",
                .action = &do_nothing,
            },
        },
        .data = NULL,
    },
    {
        .dialogue = "Un téléporteur se présente à Peter, mais impossible de savoir vers où il va!",
        .actions = {
            {
                .label = "Entrer dans le téléporteur et aller dans une pièce adjacente (possibilités égales d'avancer, revenir en arrière ou de rester au même niveau)",
                .action = &do_nothing,
            },
            {
                .label = "Dépenser 10 points de vie pour garantir d’aller à un endroit choisi",
                .action = &do_nothing,
            },
        },
        .data = NULL,
    },
    {
        .dialogue = "Un piège magique modifie le deck de Peter",
        .actions = {
            {
                .label = "Transforme tous les strikes en esquives",
                .action = &do_nothing,
            },
            {
                .label = "Transforme tous les esquives en strikes",
                .action = &do_nothing,
            },
        },
        .data = NULL,
    },
    {
        .dialogue = "La salle est vide, mais le chaudron est encore allumé, une potion est presque prête. Comment la terminer",
        .actions = {
            {
                .label = "Faire une potion de santé (hp max +10) ",
                .action = &do_nothing,
            },
            {
                .label = "Faire une potion de mana (mana max +2O)",
                .action = &do_nothing,
            },
        },
        .data = NULL,
    }};