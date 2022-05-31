#include "./../../../include/raylib.h"
#include "ending.h"
#include "./../utils/utils.h"
#include "./../credits/credits.h"
#include <stdio.h>

#define TAB_SIZE 4*2

//global variables :
extern char* creditsData[TAB_SIZE];

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;
static float creditsScrollingPosY = 0;
static Texture2D raylibLogo;

void InitEndingScreen(void)
{
    printf("Ending Screen Init\n");

    printf("screenHeight : %d  ,  GetScreenWidth : %d\n", GetScreenHeight(), GetScreenWidth());

    creditsScrollingPosY = (float)GetScreenHeight() + 20.0f;
    raylibLogo = LoadTexture("./asset/Misc/raylib_logo.png");
    
}
void UpdateEndingScreen(void)
{
    creditsScrollingPosY -= 1.5f;
    if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_DOWN))
    {
        creditsScrollingPosY -= 3.0f;
    }
    if (creditsScrollingPosY < (-GetScreenHeight() -400)) {
        creditsScrollingPosY = -GetScreenHeight() - 400;
        finishScreen = 1;
    }
}
void DrawEndingScreen(void)
{
    float scaleBackground = (float)(GetScreenWidth() / (float)background.width);
    DrawTextureEx(background, (Vector2){0, 0}, 0, scaleBackground, WHITE);

    int fontSize = 50;

    float padding = 40.0f; 
    float sizeQueue = 0.0f;
    Color color = GetColor(0x8a9696ff);
    for (int i = 0; i < TAB_SIZE-1; i+=2) {
        int factorPos = i != 0 ? i : 1;
        Vector2 sectionTitleSize = MeasureTextEx(font, creditsData[i], fontSize, 2);
        Vector2 sectionDataSize = MeasureTextEx(font, creditsData[i+1], fontSize, 2);
        sizeQueue += 20.0f;
        DrawTextEx(font, creditsData[i], (Vector2){ GetScreenWidth()/2 - sectionTitleSize.x/2, creditsScrollingPosY + sizeQueue }, fontSize, 2, color);
        sizeQueue += sectionTitleSize.y;
        DrawTextEx(font, creditsData[i+1], (Vector2){ GetScreenWidth()/2 - sectionDataSize.x/2, creditsScrollingPosY + sizeQueue }, fontSize, 2, WHITE);
        sizeQueue += sectionDataSize.y;
    }

    Vector2 sectionTitleSize = MeasureTextEx(font, "Crée avec", fontSize, 2);
    DrawTextEx(font, "Crée avec", (Vector2){ GetScreenWidth()/2 - sectionTitleSize.x/2, creditsScrollingPosY + sizeQueue + 20 }, fontSize, 2, color);
    sizeQueue += sectionTitleSize.y;
    DrawTexture(raylibLogo, GetScreenWidth()/2 - 64, creditsScrollingPosY + sizeQueue + 40, WHITE);

    fontSize = 25;
    Vector2 textSize = MeasureTextEx(font, "Appuyez sur espace\npour accélérer", fontSize, 2);
    DrawTextEx(font, "Appuyez sur espace\npour accélérer", (Vector2){ GetScreenWidth() - (textSize.x+20), GetScreenHeight()-(textSize.y+20) }, fontSize, 2, color);
    
}
void UnloadEndingScreen(void)
{
    UnloadTexture(raylibLogo);
}
int FinishEndingScreen(void)
{
    return finishScreen;
}