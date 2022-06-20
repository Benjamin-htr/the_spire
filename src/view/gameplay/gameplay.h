#ifndef GUI_GAMEPLAY
#define GUI_GAMEPLAY

#include "./../../../include/raylib.h"
#include "./../../model/game/game.h"

// Draw button with arrow in the middle
bool ArrowButton(Rectangle bounds, float rotation, int forcedState);

// Draw life bar of the charater:
void drawLifeBar();

// Draw object of character :
void drawItem(item_t *item, Texture2D texture, Vector2 position, float scaleFactor, int forcedState);

// Draw all items :
void drawItems();

// Draw choice of current event :
void drawEventChoice(event *event);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void);

// Gameplay Screen Update logic
void UpdateGameplayScreen(void);

// Gameplay Screen Draw logic
void DrawGameplayScreen(void);

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void);

// Gameplay Screen should finish?
int FinishGameplayScreen(void);

// ChangeToScreen
void ChangeToScreen(int screen);

#endif