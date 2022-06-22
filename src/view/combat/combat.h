#ifndef GUI_COMBAT
#define GUI_COMBAT

#include "./../../../include/raylib.h"
#include "./../../model/game/combat/combat.h"
#include "./../../model/game/misc/effect/effect.h"
#include "./../utils/utils.h"

// Tableau contenant les noms des effets :
extern char *EFFECT_NAME[13];
extern char *EFFECT_TEXT[5];
extern NPatchInfo cardInfo;

// Draw stats of the player :
void drawStatBoard();

// Draw one card of the hand player :
int GuiCard(card_t *card, Vector2 position, float scaleFactor, int idx, boolean isHand, int forcedState);

// Draw all cards of the hand player :
void drawHand();

//
void drawEnnemyCard();

void drawRewind();
void drawEffect(effect_t *effect, Vector2 position, float scaleFactor, boolean alignLeft, int forcedState);
void InitCombatScreen(void);
void UpdateCombatScreen(void);
void DrawCombatScreen(void);
void UnloadCombatScreen(void);
int FinishCombatScreen(void);
void loadTextureCard();
void unloadTextureCard();

#endif