#include "./../../../include/raylib.h"
#include "credits.h"
#include "./../utils/utils.h"
#include <stdio.h>

static float creditsScrollingPosY = 0;

#define TAB_SIZE 4*2

void InitCreditsScreen(void)
{
    printf("Credits Screen Init\n");

    creditsScrollingPosY = (float)GetScreenHeight() + 20.0f;
    
}
void UpdateCreditsScreen(void)
{
    creditsScrollingPosY -= 0.6f;
    if (creditsScrollingPosY < (-GetScreenHeight() + 36)) creditsScrollingPosY = -GetScreenHeight() + 36;
    
}
void DrawCreditsScreen(void)
{
    float scaleBackground = (float)(GetScreenWidth() / (float)background.width);
    DrawTextureEx(background, (Vector2){0, 0}, 0, scaleBackground, WHITE);

    int fontSize = 50;

    char *creditsData[TAB_SIZE] = { "DESIGN DU JEU",
                        "Hugo Dutoit\nHautier Benjamin",
                        "DEVELOPPEMENT DU JEU",
                        "Léonard Corre\nMartin Mille\nHugo Dutoit\nHautier Benjamin",
                        "CREATION D'ASSETS",
                        "hugo Dutoit\nHautier Benjamin",
                        "MUSIC",
                        "Dar Golan" };

    float padding = 20.0f; 
    float sizeQueue = 0.0f;
    for (int i = 0; i < TAB_SIZE-1; i+=2) {
        int factorPos = i != 0 ? i : 1;
        Vector2 sectionTitleSize = MeasureTextEx(font, creditsData[i], fontSize, 2);
        Vector2 sectionDataSize = MeasureTextEx(font, creditsData[i+1], fontSize, 2);
        sizeQueue += 20.0f;
        DrawTextEx(font, creditsData[i], (Vector2){ GetScreenWidth()/2 - sectionTitleSize.x/2, creditsScrollingPosY + sizeQueue }, fontSize, 2, GetColor(0x8a9696ff));
        sizeQueue += sectionTitleSize.y;
        DrawTextEx(font, creditsData[i+1], (Vector2){ GetScreenWidth()/2 - sectionDataSize.x/2, creditsScrollingPosY + sizeQueue }, fontSize, 2, WHITE);
        sizeQueue += sectionDataSize.y;
    }

    //DrawTextEx(font, "powered by", (Vector2){ GetScreenWidth()/2 - MeasureTextEx(font, "powered by", fontSize, 2).x/2, creditsScrollingPosY + 240 }, fontSize, 2, GRAY);
    //DrawTexture(texLogo, GetScreenWidth()/2 - 64, creditsScrollingPosY + 256, WHITE);

}
void UnloadCreditsScreen(void)
{
    
}
int FinishCreditsScreen(void)
{
    return 0;
}