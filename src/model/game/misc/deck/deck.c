#include "deck.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "time.h"

deck_t *createDeck(card_t *myCard) // fonction pour créer un deck en lui allouant de la mémoire
{
    deck_t *res = malloc(sizeof(deck_t));
    res->next = NULL;
    res->data = myCard;
    return res;
}

deck_t *copyDeck(deck_t *deck) // fonction pour copier un deck
{
    if (deck == NULL)
    {
        return createDeck(NULL);
    }

    deck_t *res = createDeck(copyCard(deck->data));
    deck_t *readingHead = deck->next;
    while (readingHead != NULL)
    {
        addCard(res, copyCard(readingHead->data));
        readingHead = readingHead->next;
    }
    return res;
}

void freeDeck(deck_t *deck)
{
    free(deck);
}

void freeDeckListAndCard(deck_t *deckList) // fonction pour libérer la mémoire en désallouant la mémoire assignée au deck et à chaque carte qu'il contient
{
    deck_t *toFree = deckList;
    deck_t *next;
    while (toFree != NULL)
    {
        next = toFree->next;
        // TODO FREE CARD
        if (toFree->data != NULL)
        {
            freeCard(toFree->data);
        }
        freeDeck(toFree);
        toFree = next;
    }
}

deck_t *createDeckFromArray(int cardIds[][2], int diffCardNumber) // retourne un deck à partir d'une liste de cartes
{
    deck_t *res = createDeck(NULL);
    for (int cardIdx = 0; cardIdx < diffCardNumber; cardIdx++)
    {
        for (int cardExemplar = 0; cardExemplar < cardIds[cardIdx][1]; cardExemplar++)
        {
            addCard(res, importCardFromId(cardIds[cardIdx][0]));
        }
    }
    return res;
}

deck_t *createRewardDeck() // retourne le deck qui contient les cartes de récompenses
{
    return createDeckFromArray(
        (int[3][2]){
            {getRandomPlayerCardId(), 1},
            {getRandomPlayerCardId(), 1},
            {getRandomPlayerCardId(), 1},
        },
        3);
}

int getPositionOfCard(deck_t *deck, char *cardName) // retourne la position d'une carte dans un deck
{
    int i = 0;
    while (deck != NULL && deck->data != NULL)
    {
        card_t *card = deck->data;
        if (strcmp(card->name, cardName) == 0)
        {
            return i;
        }
        deck = deck->next;
        i++;
    }
    return -1;
}

void addCard(deck_t *deck, card_t *card) // ajoute une carte à un deck
{
    if (deck == NULL)
    {
        deck = createDeck(card);
        return;
    }
    else
    {
        while (deck->next != NULL && deck->data != NULL)
        {
            deck = deck->next;
        }
        if (deck->data == NULL)
        {
            deck->data = card;
            return;
        }
        deck->next = createDeck(card);
    }
}

card_t *removeFirstCard(deck_t **deck) // retire la première carte d'un deck (utilisé pour piocher)
{
    if (getDeckSize(*deck) > 0)
    {
        deck_t *tmp = *deck;
        card_t *cardToReturn = tmp->data;
        *deck = (*deck)->next;
        freeDeck(tmp);
        return cardToReturn;
    }
    return NULL;
}

card_t *removeCardatIndex(deck_t **deck, int cardIdx) // retire et retourne la carte à l'indice cardIdx d'un deck
{
    if (cardIdx == 0)
    {
        return removeFirstCard(deck);
    }
    else
    {
        deck_t *current = *deck;
        int incrementor = 1;
        while (current->next != NULL)
        {
            if (incrementor == cardIdx)
            {
                deck_t *tmp = current->next;
                card_t *cardToReturn = tmp->data;
                current->next = current->next->next;
                freeDeck(tmp);
                return cardToReturn;
            }
            incrementor++;
            current = current->next;
        }
    }
    return NULL;
}

card_t *removeCard(deck_t **deck, char *cardName) // retire une carte d'un deck à partir de son nom
{
    int pos = getPositionOfCard(*deck, cardName);
    if (pos == -1)
    {
        printf("La carte n'est pas dans le deck \n");
    }
    else
    {
        return removeCardatIndex(deck, pos);
    }
    return NULL;
}

void replaceCardWithOther(deck_t **deck, CARD_ENCYCLOPEDIA_ID cardID_1, CARD_ENCYCLOPEDIA_ID cardID_2) // remplace une carte d'un deck par une autre carte
{
    char *card_1_name = CARD_ENCYCLOPEDIA[cardID_1].name;
    deck_t *readingHead = *deck;
    card_t *toFree;
    int idx = 0;
    while (readingHead != NULL && readingHead->data != NULL)
    {
        if (readingHead->data->name == card_1_name)
        {

            toFree = readingHead->data;
            readingHead->data = importCardFromId(cardID_2);
            freeCard(toFree);
        }
        readingHead = readingHead->next;
        idx++;
    }
}

card_t *draw(deck_t *deck) // pioche une carte du deck
{
    card_t *res = deck->data;
    removeFirstCard(&deck);
    return res;
}

deck_t *getElementFromDeckAtIndex(int idx, deck_t *deck) // retire un element du deck à un indice donné
{
    int i = 0;
    while (deck != NULL)
    {
        if (i == idx)
        {
            return deck;
        }
        deck = deck->next;
        i++;
    }
    return NULL;
}

int getDeckSize(deck_t *deck) // retourne la taille du deck
{
    int res = 0;
    while (deck != NULL && deck->data != NULL)
    {
        res++;
        deck = deck->next;
    }
    return res;
}

void swapElements(deck_t *deck, int n, int m) // remplace un élement du deck à l'indice n par un autre element à l'indice m, et inversement
{
    deck_t *myDeckAtM = getElementFromDeckAtIndex(m, deck);
    deck_t *myDeckAtN = getElementFromDeckAtIndex(n, deck);
    deck_t *tmp;
    if (myDeckAtM->next != NULL && myDeckAtN->next != NULL)
    {
        // on remplace les befores
        tmp = myDeckAtM->next;
        myDeckAtM->next = myDeckAtN->next;
        myDeckAtN->next = tmp;
        // on remplace les nexts
        tmp = myDeckAtM->next->next;
        myDeckAtM->next->next = myDeckAtN->next->next;
        myDeckAtN->next->next = tmp;
    }
}

void shuffleDeck(deck_t **deck) // mélange du deck
{
    int listSize = getDeckSize(*deck);
    if (listSize > 5)
    {
        int idDeck1;
        int idDeck2;
        for (int i = 0; i < listSize * 10; i++)
        {
            idDeck1 = rand() % listSize;
            idDeck2 = rand() % listSize;
            if (idDeck1 != idDeck2 && (idDeck2 - idDeck1 > 1 || idDeck1 - idDeck2 > 1))
            {
                swapElements(*deck, idDeck1, idDeck2);
            }
        } // ici on swap le premier element avec l'élement du milieu
        deck_t *beforeMiddleElement = getElementFromDeckAtIndex((rand() % (listSize - 2)) + 1, *deck);
        deck_t *middle = beforeMiddleElement->next;
        deck_t *firstNext = (*deck)->next;
        (*deck)->next = middle->next;
        middle->next = firstNext;
        beforeMiddleElement->next = *deck;
        *deck = middle;
    }
    // middleElement->next = deck;
    // beforeMiddleElement->next = deck;
    // deck->next = afterMiddleElement;
    // deck = middleElement;
}

void testDeck() // fonction du test de deck
{
    printf("\n==============================\n\tTEST DE DECK\n==============================\n");
    deck_t *myDeck = createDeckFromArray((int[][2]){{DODGE_A, 4}, {PULVERIZE, 3}, {DEFENSE, 3}, {JAWURM2_HAIRPULLING, 3}, {SPECTRUM, 3}}, 5);
    deck_t *copyedDeck = copyDeck(myDeck);
    displayDeck(myDeck);
    removeFirstCard(&myDeck);
    displayDeck(myDeck);
    removeCardatIndex(&myDeck, 3);
    displayDeck(myDeck);
    shuffleDeck(&myDeck);
    displayDeck(myDeck);
    freeDeckListAndCard(myDeck);
    displayDeck(copyedDeck);
    freeDeckListAndCard(copyedDeck);
}

void displayDeck(deck_t *myDeck) // affiche le deck
{
    printf("\n=======================\n");
    int i = 0;
    if (myDeck == NULL || myDeck->data == NULL)
    {
        printf("The deck is empty \n");
    }
    while (myDeck != NULL && myDeck->data != NULL)
    {
        card_t *myCard = myDeck->data;
        printf("Element %d %s: \n", i, myCard->name);
        myDeck = myDeck->next;
        i++;
    }
    printf("=======================\n");
}
