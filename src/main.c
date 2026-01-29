/*******************************************************************************************
* Circles Game - Float Physics Edition
* Includes: State Machine, Ball-to-Ball Collision, Drag, and Smooth Gravity
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>         
#include <math.h>           

//----------------------------------------------------------------------------------
// Global Variables and Constants
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef enum GameState {
    STATE_MAINMENU,
    STATE_GAMEPLAY,
    STATE_GAMEOVER
} GameState;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    GameState currentState = STATE_GAMEPLAY;
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(screenWidth - 80, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
