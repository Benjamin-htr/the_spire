#ifndef GUI_COMBAT
#define GUI_COMBAT

#include "./../../../include/raylib.h"

void drawStatBoard();
int GuiCard(Vector2 position, float scaleFactor, int forcedState);
void InitCombatScreen(void);
void UpdateCombatScreen(void);
void DrawCombatScreen(void);
void UnloadCombatScreen(void);
int FinishCombatScreen(void);

#endif