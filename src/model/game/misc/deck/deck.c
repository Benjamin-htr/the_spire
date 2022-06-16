#include "deck.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "time.h"

deck_t *createDeck(card_t *myCard)
{
    deck_t *res = malloc(sizeof(deck_t) *5);
    res->next = NULL;
    res->data = myCard;
    return res;
}

deck_t *createDeckFromArray(int cardIds[][2], int diffCardNumber)
// first int is the card id 2nd one is the number of card
// 2nd Param is just the size of the array
{
    deck_t *res = createDeck(NULL);
    printf("ON ARRIVE A CREER LE DECK DE MORT \n");
    fflush(stdout);
    for (int cardIdx = 0; cardIdx < diffCardNumber; cardIdx++)
    {
        for (int cardExemplar = 0; cardExemplar < cardIds[cardIdx][1]; cardExemplar++)
        {
            card_t * card = importCardFromId(cardIds[cardIdx][0]);
            printf("CREATION DE LA CARTE MARCHE");
            fflush(stdout);
            addCard(res, card);
            printf("LE ADD CARD MARCHE \n");
            fflush(stdout);
        }
    }
    printf("LE ADD CARD MARCHE A LA FIN\n");
            fflush(stdout);
    return res;
}

void addCard(deck_t *deck, card_t *card)
{
    printf("add CARD CALLED \n");
    fflush(stdout);
    if (deck == NULL)
        {
            printf("DECK IS NULL \n");
            fflush(stdout);
            deck = createDeck(card);
            return;
    }
    if (deck->data == NULL)
    {
         printf("DATAK IS NULL \n");
    fflush(stdout);
        deck->data = card;
    }
    else
    {
         if (deck == NULL)
        {
            printf("DECK IS NULL, ON VA RETURN \n");
            fflush(stdout);
            deck = createDeck(card);
            return;
           }
        
        while (deck !=NULL && deck->next != NULL)
        {
             printf("On rentre dans le putain de whileL \n");
            fflush(stdout);
            deck = deck->next;
            printf(" DECK = DECK NEXT FAIT BUGUER ? IMPOSSIBLE MEC\n");
            fflush(stdout);
            if(deck ==NULL){
                 printf("CEC HIEN DE DECK EST NULLn \n");
                  fflush(stdout); 
                  deck->next = NULL;
                  printf("LA CA VA CRASH CHIEN \n");
                  fflush(stdout);
            }
            displayDeck(deck);
                  fflush(stdout);
              printf("CEC HIEN DE DECK EST NULLn 45 \n");
                  fflush(stdout); 
                  
        }
        printf("SORTIE DU WHILE\n");
        fflush(stdout);
        deck->next = createDeck(card);
    }
     printf("SORTIE DE ADD CARD \n");
    fflush(stdout);
}

deck_t *removeFirstCard(deck_t *deck)
{
    if (deck == NULL)
        return createDeck(NULL);
    deck = deck->next;
    if(deck ==NULL){
        deck = createDeck(NULL);
    }
    return deck;
}


int getPositionOfCard(deck_t *deck, char *cardName){
    int i =0;
    while(deck!=NULL && deck->data!=NULL){
        card_t* card = deck->data;
        if(strcmp(card->name,cardName)==0){
            return i;
        }
        deck=deck->next;
        i++;
    }
    return -1;
}

void removeCard(deck_t **deck, char *cardName){
    int pos = getPositionOfCard(*deck,cardName);
    deck_t * tmp ;
    if(pos==-1){
        printf("La carte n'est pas dans le deck \n");
        fflush(stdout);
    }
    else {
        if(pos==0){
            if(size(*deck)>0){
                tmp = *deck;
                *deck = (*deck)->next;
                free(tmp);
                if(*deck==NULL){
                    *deck = createDeck(NULL);
                }
                return; 
            }
        }
        else {
            deck_t* current = *deck;
            while(current->next!=NULL){
                card_t * myCard =  current->next->data;
                if(strcmp(myCard->name,cardName)==0){
                    tmp=current->next;
                    current->next = current->next->next;
                 //   free(tmp);
                    break;
                }
                else {
                    current = current->next;
                }
            }
        }
}
}

card_t *draw(deck_t *deck)
{
    card_t *res = deck->data;
    deck = deck->next;
    return res;
}

deck_t *getElementFromDeckAtIndex(int idx, deck_t *deck)
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
    return deck;
}

int size(deck_t *deck)
{
    int res = 0;
    while (deck != NULL && deck->data!=NULL)
    {
        res++;
        deck = deck->next;
    }
    return res;
}

void swapElements(deck_t *deck, int n, int m)
{
    if (n > 0 && m > 0)
    {
        deck_t *myDeckBeforeN = getElementFromDeckAtIndex(n - 1, deck);
        deck_t *myDeckAtM = getElementFromDeckAtIndex(m, deck);
        deck_t *myDeckAtN = getElementFromDeckAtIndex(n, deck);
        deck_t *deckAfterM = getElementFromDeckAtIndex(m + 1, deck);
        deck_t *deckAfterN = getElementFromDeckAtIndex(n + 1, deck);
        deck_t *myDeckBeforeM = getElementFromDeckAtIndex(m - 1, deck);
        myDeckBeforeN->next = myDeckAtM;
        myDeckAtM->next = deckAfterN;
        myDeckBeforeM->next = myDeckAtN;
        myDeckAtN->next = deckAfterM;
    }
}

deck_t *shuffleDeck(deck_t *deck)
{
    displayDeck(deck);
    fflush(stdout);
    int listSize = size(deck);
    srand(time(NULL));
    int number1;
    int number2;
    for (int i = 0; i < listSize * 10; i++)
    {
        number1 = rand() % listSize;
        number2 = rand() % listSize;
        if (number1 != number2 && (number2 - number1 > 1 || number1 - number2 > 1))
        {
            swapElements(deck, number1, number2);
        }
    }
    // ici on swap le premier element avec l'élement du milieu
    deck_t *middleElement = getElementFromDeckAtIndex((int)(listSize / 2), deck);
    deck_t *beforeMiddleElement = getElementFromDeckAtIndex((int)(listSize / 2) - 1, deck);
    deck_t *afterMiddleElement = getElementFromDeckAtIndex((int)(listSize / 2) + 1, deck);
    middleElement->next = deck->next;
    beforeMiddleElement->next = deck;
    deck->next = afterMiddleElement;
    deck = middleElement;
    return deck;
}

void testDeck()
{
    // deck_t *myDeck = createDeck(&CARD_DODGE);
    // addCard(myDeck, &PULVERIZE);
    // addCard(myDeck, &BLOUNI_JAB);
    // addCard(myDeck, &ACCELERATION);
    // addCard(myDeck, &DEFENSE);
    // addCard(myDeck, &BLOUNI_KICK);
    // myDeck = shuffleDeck(myDeck);
    // displayDeck(myDeck);
    deck_t *myDeck = createDeckFromArray((int[][2]){{DODGE_A, 1}, {PULVERIZE, 2}, {DEFENSE, 3}}, 3);
    displayDeck(myDeck);
}

void displayDeck(deck_t *myDeck)
{
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
}
