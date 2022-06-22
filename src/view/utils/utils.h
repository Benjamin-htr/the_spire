#include "./../../../include/raylib.h"

#ifndef GUI_UTILS
#define GUI_UTILS

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen
{
    MENU = 0,
    CREDITS,
    GAMEPLAY,
    COMBAT_SCREEN,
    ENDING
} GameScreen;

typedef struct Sprite
{
    Texture2D texture;
    int nbFramesPerLine;
    int nbLines;
    Vector2 position;
    Rectangle frameRec;
    int framesCounter;
    int currentLine;
    int currentFrame;
} Sprite;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Texture2D background;
extern Sound buttonSound;
extern Texture2D buttonPatch;
extern NPatchInfo buttonInfo;
extern bool shouldClose;
extern bool showInGameMenu;

extern float transAlpha;
extern bool onTransition;
extern bool transFadeOut;
extern int transFromScreen;
extern int transToScreen;

//----------------------------------------------------------------------------------
// Functions declarations
//----------------------------------------------------------------------------------
// Init a sprite :
void constructSprite(Sprite *sprite, char *texturePath, int nbFramesPerLine, int nbLines);

// Update sprite
void updateSprite(Sprite *sprite);

// Draw sprite :
void drawSprite(Sprite *sprite, Vector2 position, float angle, float scale, Color c);

// Gui button (immediate mode, update and draw)
bool GuiButton(Rectangle bounds, const char *text, int forcedState);

// Transitions :
void TransitionToScreen(int screen); // Request transition to next screen

// Draw in game menu
void drawInGameMenu();

// Draw text inside a rectangle and wrap it if too big
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);

#endif