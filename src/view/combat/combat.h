#ifndef GUI_COMBAT
#define GUI_COMBAT

#include "./../../../include/raylib.h"
#include "./../../model/game/combat/combat.h"
#include "./../utils/utils.h"

void drawStatBoard();
int GuiCardHand(card_t *card, Vector2 position, float scaleFactor, int forcedState);
void InitCombatScreen(void);
void UpdateCombatScreen(void);
void DrawCombatScreen(void);
void UnloadCombatScreen(void);
int FinishCombatScreen(void);

#endif