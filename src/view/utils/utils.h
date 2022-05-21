#include "./../../../include/raylib.h"


#ifndef GUI_UTILS
#define GUI_UTILS

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { MENU = 0, CREDITS, GAMEPLAY, ENDING } GameScreen;


//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;
extern Texture2D background;

#endif