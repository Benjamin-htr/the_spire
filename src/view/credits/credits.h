#ifndef GUI_CREDITS
#define GUI_CREDITS

// Init credits screen (variables for examples) :
void InitCreditsScreen(void);

// Update variables in credits screen (continuously call when on credits screen) :
void UpdateCreditsScreen(void);

// Draw credits screen (continuously call when on credits screen):
void DrawCreditsScreen(void);

// Unload all textures from credits screen and free dynamic allocated pointer :
void UnloadCreditsScreen(void);

// Return 1 if credits screen is finish :
int FinishCreditsScreen(void);

#endif