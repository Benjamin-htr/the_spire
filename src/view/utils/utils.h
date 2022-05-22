#include "./../../../include/raylib.h"


#ifndef GUI_UTILS
#define GUI_UTILS

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { MENU = 0, CREDITS, GAMEPLAY, ENDING } GameScreen;

typedef struct Sprite {
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
extern Sound fxCoin;
extern Texture2D background;


//----------------------------------------------------------------------------------
// Functions declarations
//----------------------------------------------------------------------------------
void constructSprite(Sprite* sprite, char* texturePath, int nbFramesPerLine, int nbLines, Vector2 position);

void updateSprite(Sprite* sprite);

void drawSprite(Sprite* sprite, Vector2 position, float angle, float scale, Color c);

#endif