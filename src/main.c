/*******************************************************************************************
*
*  The Spire.
*
*******************************************************************************************/

#include "stdio.h"
#include "./../include/raylib.h"
#include "./view/menu/menu.h"
#include "./view/credits/credits.h"
#include "./view/ending/ending.h"
#include "./view/gameplay/gameplay.h"
#include "./view/utils/utils.h"

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
//----------------------------------------------------------------------------------
GameScreen currentScreen = 0;
Font font = { 0 };
Music music = { 0 };
Texture2D background = { 0 };
Sound buttonSound = { 0 };
Texture2D buttonPatch = { 0 };
NPatchInfo buttonInfo = { 0 };
bool shouldClose = false;

// Required variables to manage screen transitions (fade-in, fade-out)
float transAlpha = 0.0f;
bool onTransition = false;
bool transFadeOut = false;
int transFromScreen = -1;
int transToScreen = -1;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static const int screenWidth = 1200;
static const int screenHeight = 675;

// NOTE: Some global variables that require to be visible for all screens,
// are defined in utils.h (i.e. currentScreen)

//----------------------------------------------------------------------------------
// Module Functions Declaration (local)
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame


int main(void)
{
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);

    // Initialization (OpenGL context)
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "The Spire");

    // Global data loading (assets that must be available in all screens, i.e. fonts)
    InitAudioDevice();

    font = LoadFontEx("./asset/Misc/Fonts/pixantiqua.ttf", 24, 0, 0);
    background = LoadTexture("./asset/Misc/background.png");

    music = LoadMusicStream("./asset/Misc/Audio/Music/Video-Game-Music-Dar-Golan-200bp.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.1f);

    buttonSound = LoadSound("./asset/Misc/Audio/Sound/Confirm.wav");

    buttonPatch = LoadTexture("./asset/Misc/button.png");
    buttonInfo.source = (Rectangle){ 0, 0, 115, buttonPatch.height },
    buttonInfo.left = 60;
    buttonInfo.top = 60;
    buttonInfo.right = 60;
    buttonInfo.bottom =60;

    // Setup and Init first screen
    currentScreen = MENU;
    InitMenuScreen();
    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //-------------------------------------------------------------------------------------
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!(WindowShouldClose() || shouldClose))    // Detect window close button or ESC key
    {
        // Update
        UpdateDrawFrame();
        
        // check for alt + enter
 		if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
 		{
 			//int display = GetCurrentMonitor();
 
            
            //if (IsWindowFullscreen())
            //{
            //    printf("was fullscreen : screenWidth : %d, screenHeight : %d \n ", screenWidth, screenHeight);
            //    // if we are full screen, then go back to the windowed size
            //    SetWindowSize(screenWidth, screenHeight);
            //}
            //else
            //{
            //    
            //    // if we are not full screen, set the window size to match the monitor we are on
            //    SetWindowSize(GetMonitorPhysicalWidth(display), GetMonitorPhysicalHeight(display));
            //}
 
            // toggle the state
 			ToggleFullscreen();

             printf("screenWidth : %d, screenHeight : %d \n ", GetScreenWidth(), GetScreenHeight());
         }
    }

    // Unload all global loaded data (i.e. fonts) here!
    UnloadFont(font);
    UnloadTexture(background);
    UnloadMusicStream(music);

    CloseAudioDevice();     // Close audio context
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}


//----------------------------------------------------------------------------------
// Module Functions Definition (local)
//----------------------------------------------------------------------------------
// Change to screen, no transition
static void ChangeToScreen(int screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case MENU: UnloadMenuScreen(); break;
        case CREDITS: UnloadCreditsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case MENU: InitMenuScreen(); break;
        case CREDITS: InitCreditsScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Update transition effect
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
                case MENU: UnloadMenuScreen(); break;
                case CREDITS: UnloadCreditsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
                case MENU: InitMenuScreen(); break;
                case CREDITS: InitCreditsScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case MENU:
            {
                UpdateMenuScreen();

                if (FinishMenuScreen())
                {
                    //TransitionToScreen(GAMEPLAY);
                    PlayMusicStream(music);
                }

            } break;
            case CREDITS:
            {
                UpdateCreditsScreen();

                if (FinishCreditsScreen() == 1) TransitionToScreen(MENU);
                //else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                //if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

            } break;
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen() == 1) TransitionToScreen(MENU);

            } break;
            default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case MENU: DrawMenuScreen(); break;
            case CREDITS: DrawCreditsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

    EndDrawing();
    //----------------------------------------------------------------------------------
}