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

const int DEFAULT_PADDLE_SPEED                  = 5;
const int DEFAULT_PADDLE_WIDTH                  = 75;
const int DEFAULT_PADDLE_HEIGHT                 = 15;

const int DEFAULT_BALL_SPEED                    = 1;
const int DEFAULT_BALL_RADIUS                   = 7;

const int INIT_WIDTH_BRICKS                     = 30;
const int INIT_HEIGHT_BRICKS                    = 10;
const int INIT_NUMBER_BRICKS                    = 20;

const int BOUNDARY_THICKNESS                    = 15;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct Ball {
    int     posX;
    bool    dirX;              
    int     posY;   
    bool    dirY;            
    int     velocity;
    int     angle;           
    int     radius;             
    Color   color;              
    float   counter;           
    bool    alive;              
} Ball;

typedef struct Paddle {
    int     posX;               
    int     posY;
    int     width;             
    int     height;          
    float   velocity;                      
    Color   color;              
    float   counter;           
    bool    alive;              
} Paddle;

typedef struct Brick {
    int     posX;            
    int     posY; 
    int     width;             
    int     height;                              
    Color   color;              
    float   counter;           
    bool    alive;              
} Brick;

typedef enum GameState {
    STATE_MAINMENU,
    STATE_GAMEPLAY,
    STATE_GAMEOVER
} GameState;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

void movePlayer(Paddle *player, int screenHeight, int screenWidth);
Brick initBricks(int posX, int posY, int width, int height, Color color);
Ball createBall(float posX, float posY, Color color);
//bool ImpactX(Ball *b, int screenWidth, int screenHeight);
//bool ImpactY(Ball *b, int screenWidth, int screenHeight);
void moveBall(Ball *b, int screenWidth, int screenHeight);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 550;

    InitWindow(screenWidth, screenHeight, "Brick Breaker");

    //GameState currentState = STATE_GAMEPLAY;
    SetTargetFPS(60);

    // Init the player paddle
    Paddle Player;
        Player.posX             = screenWidth / 2.0f;     
        Player.posY             = screenHeight - (float)(BOUNDARY_THICKNESS * 2);
        Player.width            = DEFAULT_PADDLE_WIDTH;
        Player.height           = DEFAULT_PADDLE_HEIGHT;
        Player.velocity         = DEFAULT_PADDLE_SPEED;              
        Player.color            = BLUE; 
        Player.counter          = 0.0f;   
        Player.alive            = true;

    // Init Ball
    Ball Ball;
        Ball.posX               = screenWidth / 2.0f;
        Ball.posY               = screenHeight / 2.0f;
        Ball.dirX               = 1;
        Ball.dirY               = 0;
        Ball.radius             = DEFAULT_BALL_RADIUS;
        Ball.color              = WHITE;
        Ball.velocity           = DEFAULT_BALL_SPEED;
        Ball.counter            = 0.0f;
        Ball.alive              = true;


    for (int i = 0; i < INIT_NUMBER_BRICKS; i++) 
    {
        for (int j = 0; j < INIT_NUMBER_BRICKS / 5; j++) 
        {
        
        }
    }

        // Brick bricks[INIT_NUMBER_BRICKS];
        // bricks[i] = initBricks( (i * INIT_WIDTH_BRICKS + 2) + BOUNDARY_THICKNESS + 5 , BOUNDARY_THICKNESS * 5, 50, 20, RED);
        // //bricks[i] = initBricks( (i * INIT_WIDTH_BRICKS + 2) + BOUNDARY_THICKNESS + 5 , BOUNDARY_THICKNESS * 5, 50, 20, RED);

    //initBricks(int posX, int posY, int width, int height, Color color) 

    while (!WindowShouldClose())
    {

        // Update Player Position
        movePlayer(&Player, screenHeight, screenWidth);
        moveBall(&Ball, screenWidth, screenHeight);

        // Update the Screen
        BeginDrawing();

            //Draw environment
            ClearBackground(BLACK);
            DrawRectangle(0, 0, screenWidth, BOUNDARY_THICKNESS, WHITE);
            DrawRectangle(screenWidth - BOUNDARY_THICKNESS, 0, BOUNDARY_THICKNESS, screenHeight, WHITE);
            DrawRectangle(0, 0, BOUNDARY_THICKNESS, screenHeight, WHITE);
            DrawRectangle(0, screenHeight - BOUNDARY_THICKNESS, screenWidth, BOUNDARY_THICKNESS, WHITE);
            
            // Menu Bar
            DrawRectangle(0, BOUNDARY_THICKNESS * 3, screenWidth, BOUNDARY_THICKNESS, WHITE);
            DrawRectangle(BOUNDARY_THICKNESS, BOUNDARY_THICKNESS, screenWidth - (BOUNDARY_THICKNESS * 2), BOUNDARY_THICKNESS * 2, BLACK);
        
            // Draw Player Paddle
            DrawRectangle(Player.posX, Player.posY, Player.width, Player.height, Player.color);

            // Draw Ball
            DrawCircle(Ball.posX, Ball.posY, Ball.radius, Ball.color);

            // Draw Bricks

            DrawFPS(screenWidth - 80 - BOUNDARY_THICKNESS, 5 + BOUNDARY_THICKNESS);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


//------------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------------

void movePlayer(Paddle *player, int screenHeight, int screenWidth)
{
    if (IsKeyDown(KEY_D)) 
        if( (player->posX + player->width) < (screenWidth - BOUNDARY_THICKNESS) )
        {
            if ( (player->velocity + player->posX + player->width) < (screenWidth - BOUNDARY_THICKNESS) )
                player->posX += player->velocity;
            else
                player->posX = (screenWidth - BOUNDARY_THICKNESS) - player->width - 1;
        }

    if (IsKeyDown(KEY_A)) 
        if( (player->posX) > BOUNDARY_THICKNESS )
        {    if ( (player->posX - player->velocity) > BOUNDARY_THICKNESS )
                player->posX -= player->velocity;
            else
                player->posX = BOUNDARY_THICKNESS + 1;
        }
}

Brick initBricks(int posX, int posY, int width, int height, Color color) 
{
    Brick b;
    b.posX              = posX;
    b.posY              = posY;
    b.width             = width;
    b.height            = height;
    b.color             = color;
    b.counter           = 0.0f;
    b.alive             = true;
    
    return b;
}

Ball createBall(float posX, float posY, Color color) 
{
    Ball b;
    b.posX          = posX;
    b.dirX          = 0;
    b.posY          = posY;
    b.dirY          = 0;
    b.radius        = DEFAULT_BALL_RADIUS;
    b.color         = color;
    b.velocity      = DEFAULT_BALL_SPEED;
    b.counter       = 0.0f;
    b.alive         = true;

    return b;
}

void moveBall(Ball *b, int screenWidth, int screenHeight)
{
    // Sign Shifts
    bool impactY = false;
    bool impactX = false;

    // Check for X Boundary Impact - Right Wall
    if ( b->dirX == 1 && ( (b->posX + b->radius) >= (screenWidth - BOUNDARY_THICKNESS) ) )
    {
        impactX = true;
        b->dirX = !b->dirX;

        if( (b->posX + b->radius + b->velocity) > (screenWidth - BOUNDARY_THICKNESS) )
            b->posX += b->velocity;
        else
            b->posX = screenWidth - BOUNDARY_THICKNESS - b->radius; // touch the wall            
    }
    
    // Check for X Boundary Impact - Left Wall
    else if ( b->dirX == 0 &&  ( (b->posX - b->radius) <= (BOUNDARY_THICKNESS) ) )
    {
        impactX = true;
        b->dirX = !b->dirX;

        if( (b->posX - b->radius - b->velocity) < (BOUNDARY_THICKNESS) )
            b->posX -= b->velocity;
        else
            b->posX = BOUNDARY_THICKNESS + b->radius; // touch the wall            
    } 

     // Check for Y Boundary Impact - Bottom Wall
    if ( b->dirY == 1 && ( (b->posY + b->radius) >= (screenHeight - BOUNDARY_THICKNESS) ) )
    {
        impactY = true;
        b->dirY = !b->dirY;

        if( (b->posY + b->radius + b->velocity) > (screenHeight - BOUNDARY_THICKNESS) )
            b->posY += b->velocity;
        else
            b->posY = screenHeight - BOUNDARY_THICKNESS - b->radius; // touch the wall            
    }       

    // Check for Y Boundary Impact - Top Wall
    else if ( b->dirY == 0 && ( (b->posY - b->radius) <= (BOUNDARY_THICKNESS * 4) ) )
    {
        impactY = true;
        b->dirY = !b->dirY;

        if( (b->posY - b->radius - b->velocity) < (BOUNDARY_THICKNESS * 4) )
            b->posY -= b->velocity;
        else
            b->posY = (BOUNDARY_THICKNESS * 4) + b->radius; // touch the wall            
    }
    
    if (!impactX)
    {
        if (b->dirX == 1)
            b->posX += b->velocity;
        else
            b->posX -= b->velocity;
    }

    if(!impactY)
    {
        if (b->dirY == 1)
            b->posY += b->velocity;
        else
            b->posY -= b->velocity;
    }
}

