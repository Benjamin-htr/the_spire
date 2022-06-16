#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "event.h"

int NB_EVENT = 4;

event *event_init(char* dialogue, option actions[], void* data)
{
    event* e = malloc(sizeof(event));
    e->dialogue = dialogue;
    e->actions[0] = actions[0];
    e->actions[1] = actions[1];
    e->data = data; // mettre un sanctuaire ici 
    return e;
}


event *importEvent(event_import event_import_){
    return event_init(event_import_.dialogue,event_import_.actions,event_import_.data);
}

event *get_random_event(){
    srand(time(NULL));
    return importEvent(EVENT_ENCYCLOPEDIA[((int) (rand()%NB_EVENT))]);
}

event_import EVENT_ENCYCLOPEDIA[] = {
    {
        .dialogue = "Peter décourvre un miniboss entrain de dormir",
        .actions = {
            {
                "Lancer un combat contre un miniboss (gains normaux en cas de victoire)",
                NULL,
            },
            {
                "Ne pas faire le combat et avancer normalement",
                NULL,
            },
        },
        .data = NULL, // get random miniboss if event is miniboss
    },
    {
        .dialogue = "Un téléporteur se présente à Peter, mais impossible de savoir vers où il va!",
        .actions = {
            {
                .label = "Entrer dans le téléporteur et aller dans une pièce adjacente (possibilités égales d'avancer, revenir en arrière ou de rester au même niveau)",
                .action = NULL,
            },
            {
                .label = "Dépenser 10 points de vie pour garantir d’aller à un endroit choisi",
                .action = NULL,
            }
        },
        .data = NULL,
    },
    {
        .dialogue ="Un piège magique modifie le deck de Peter",
        .actions = {
            {
                .label = "Transforme tous les strikes en esquives",
                .action = NULL,
            },
            {
                .label = "Transforme tous les esquives en strikes",
                .action = NULL,
            }
        },
        .data = NULL,   
    },
    {
        .dialogue ="La salle est vide, mais le chaudron est encore allumé, une potion est presque prête. Comment la terminer",
        .actions = {
            {
                .label = "Faire une potion de santé (hp max +10) ",
                .action = NULL,
            },
            {
                .label = "Faire une potion de mana (mana max +2O",
                .action = NULL,
            }
        },
        .data = NULL,  
    }
};