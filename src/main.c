/*******************************************************************************************
* Circles Game - Float Physics Edition
* Includes: State Machine, Ball-to-Ball Collision, Drag, and Smooth Gravity
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>         
#include <math.h>

#define INIT_ROWS_OF_BRICKS 6
#define INIT_COLS_OF_BRICKS 10

//----------------------------------------------------------------------------------
// Global Variables and Constants
//----------------------------------------------------------------------------------

const int DEFAULT_PADDLE_SPEED                  = 5;
const int DEFAULT_PADDLE_WIDTH                  = 75;
const int DEFAULT_PADDLE_HEIGHT                 = 20;

const int DEFAULT_BALL_SPEED                    = 5;
const int DEFAULT_BALL_RADIUS                   = 7;

const int INIT_WIDTH_BRICKS                     = 80;
const int INIT_HEIGHT_BRICKS                    = 25;
const int BRICK_WIDTH_PADDING                   = 5;
const int BRICK_HEIGHT_PADDING                  = 5;

const int BOUNDARY_THICKNESS                    = 15;

const int INIT_PLAYER_LIVES                     = 3;

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
    int     score;
    int     lives;              
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
    SCREEN_SAVER,
    STATE_MAINMENU,
    STATE_GAMEPLAY,
    STATE_GAMEOVER,
    DEMO_MODE
} GameState;

//Array of bricks
Brick bricks[INIT_ROWS_OF_BRICKS * INIT_COLS_OF_BRICKS];

// 3D Array of bricks
// Currently hard coded, will soon change over to utilize screen wdith/height based parsing
// With buffers and brick dimensions

// }
//     { {}, {}, {}, {}, {} },
//     { {}, {}, {}, {}, {} }, 
//     { {}, {}, {}, {}, {} }, 
//     { {}, {}, {}, {}, {} }, 
//     { {}, {}, {}, {}, {} }
// };

int brickPositions[INIT_ROWS_OF_BRICKS][INIT_COLS_OF_BRICKS][2];

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

void movePlayer(Paddle *player, int screenHeight, int screenWidth);
Brick initBricks(int posX, int posY, int width, int height, Color color);
Ball createBall(float posX, float posY, Color color);
void moveBall(Ball *b, Paddle *p, int screenWidth, int screenHeight);
void initPlayer(Paddle *p, int screenWidth, int screenHeight);
void initBall(Ball *b, int screenWidth, int screenHeight);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 550;
    int currentScreen = STATE_MAINMENU;

    InitWindow(screenWidth, screenHeight, "Brick Breaker");

    //GameState currentState = STATE_GAMEPLAY;
    SetTargetFPS(60);

    // Init the player paddle
    Paddle Player;
    initPlayer(&Player, screenWidth, screenHeight);

    // Init Ball
    Ball Ball;
    initBall(&Ball, screenWidth, screenHeight);


    //Init Bricks
    int brickIndex = 0; //counter for storing bricks in array
    for (int i = 0; i < INIT_ROWS_OF_BRICKS; i++)
    {
        for(int j = 0; j < INIT_COLS_OF_BRICKS - 1; j++)
        {
            brickPositions[i][j][0] = (j * INIT_WIDTH_BRICKS) + (j * BRICK_WIDTH_PADDING) + BOUNDARY_THICKNESS + BRICK_WIDTH_PADDING;
            brickPositions[i][j][1] = (i * INIT_HEIGHT_BRICKS) + (i * BRICK_HEIGHT_PADDING) + BOUNDARY_THICKNESS * 4 + BRICK_WIDTH_PADDING;

            bricks[brickIndex] = initBricks( brickPositions[i][j][0], brickPositions[i][j][1], INIT_WIDTH_BRICKS, INIT_HEIGHT_BRICKS, RED);
        }
    }

    // Array of the brick colors per row
    Color brickColors[INIT_ROWS_OF_BRICKS] = { PURPLE, ORANGE, RED, YELLOW, GREEN, BLUE };

    while (!WindowShouldClose())
    {
        switch (currentScreen) 
        {
            case SCREEN_SAVER:
                break;

            case STATE_MAINMENU:
                currentScreen = STATE_GAMEPLAY;
                Player.lives = INIT_PLAYER_LIVES;
                initPlayer(&Player, screenWidth, screenHeight);
                initBall(&Ball, screenWidth, screenHeight);
                break;

            case STATE_GAMEPLAY:

                // Update Player Position
                movePlayer(&Player, screenHeight, screenWidth);
                moveBall(&Ball, &Player, screenWidth, screenHeight);

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
                    brickIndex = 0; //clear brick counter
                    for (int i = 0; i < INIT_ROWS_OF_BRICKS; i++) 
                    {
                        for (int j = 0; j < INIT_COLS_OF_BRICKS - 1; j++) 
                        {
                            if(bricks[brickIndex].alive)
                                DrawRectangle(brickPositions[i][j][0], brickPositions[i][j][1], INIT_WIDTH_BRICKS, INIT_HEIGHT_BRICKS, brickColors[i]);
                        }
                    }

                    DrawFPS(screenWidth - 80 - BOUNDARY_THICKNESS, 5 + BOUNDARY_THICKNESS);
                    DrawText(TextFormat("Lives: %i", Player.lives), 80 + BOUNDARY_THICKNESS, 5 + BOUNDARY_THICKNESS, 20, WHITE);
                    //DrawText(TextFormat("Lives: %i", Player.lives), screenWidth - 80 - BOUNDARY_THICKNESS, 5 + BOUNDARY_THICKNESS, 20, WHITE);
                EndDrawing();

                if (Player.lives <= 0)
                    currentScreen = STATE_GAMEOVER;

                break;

            case STATE_GAMEOVER:

                if (IsKeyDown(KEY_ENTER))
                    currentScreen = STATE_MAINMENU;

                // Update the Screen
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 50, 40, RED);
                DrawText("Press ENTER to return to Main Menu", screenWidth/2 - 200, screenHeight/2 + 10, 20, WHITE);
                EndDrawing();
                
                break;

            default:
                break;
        }

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

void moveBall(Ball *b, Paddle *p,int screenWidth, int screenHeight)
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
        p->lives -= 1;
        impactY = true;
        b->dirY = !b->dirY;

        //reset ball to center of screen
        initBall(b, screenWidth / 2.0f, screenHeight / 2.0f);

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

    // Check for impact with bricks
    // If in region where bricks could even be (Below last layer of bricks + brick spacing margin)
    if (b->posY - b->radius <= brickPositions[INIT_ROWS_OF_BRICKS][INIT_COLS_OF_BRICKS - 1][1] + INIT_HEIGHT_BRICKS + BRICK_HEIGHT_PADDING)
    {
        int brickIndex = 0; //clear brick counter
        for (int i = 0; i < INIT_ROWS_OF_BRICKS; i++) 
        {
            for (int j = 0; j < INIT_COLS_OF_BRICKS - 1; j++) 
            {
                if(bricks[brickIndex].alive)
                    DrawRectangle(brickPositions[i][j][0], brickPositions[i][j][1], INIT_WIDTH_BRICKS, INIT_HEIGHT_BRICKS, RED);
            }
        }
    }
    
    // Check for a colision with the paddle
    // If in lower region of map where the paddle could even be
    if (b->posY + b->radius + b->velocity >= p->posY )
    {
        // Check if above paddle
        if(b->posY + b->radius > p->posY)
        {

            // Check if within paddle X bounds
            if( (b->posX + b->radius>= p->posX) && (b->posX - b->radius<= p->posX + p->width) )
            {
            impactY = true;
            b->dirY = !b->dirY;

            if( (b->posY - b->radius - b->velocity) < (p->posY) )
                b->posY -= b->velocity;
            else
                b->posY = (p->posY) - b->radius; // touch the paddle
            }

            // // Check if hitting left side of paddle
            // else if( (b->posX + b->radius + b->velocity >= p->posX) && (b->posX < p->posX) && b->dirX == 1 )
            // {
            //     impactX = true;
            //     b->dirX = 0; //move left

            //     if( (b->posX + b->radius + b->velocity) < (p->posX) )
            //         b->posX -= b->velocity;
            //     else
            //         b->posX = p->posX - b->radius; // touch the left side of paddle            
            // }
            // {
            //     impactX = true;
            //     b->dirX = 0; //move left

            //     if( (b->posX + b->radius + b->velocity) > (p->posX) )
            //         b->posX += b->velocity;
            //     else
            //         b->posX = p->posX - b->radius; // touch the paddle            
            // }
        }
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


void initPlayer(Paddle *p, int screenWidth, int screenHeight)
{
    p->posX             = screenWidth / 2.0f;     
    p->posY             = screenHeight - (float)(BOUNDARY_THICKNESS * 2) - DEFAULT_PADDLE_HEIGHT / 2.0f;
    p->width            = DEFAULT_PADDLE_WIDTH;
    p->height           = DEFAULT_PADDLE_HEIGHT;
    p->velocity         = DEFAULT_PADDLE_SPEED;              
    p->color            = BLUE; 
    p->counter          = 0.0f;   
    p->alive            = true;
    p->score            = 0;
    p->lives            = INIT_PLAYER_LIVES;
}

void initBall(Ball *b, int screenWidth, int screenHeight)
{
    b->posX          = screenWidth / 2.0f;
    b->posY          = screenHeight / 2.0f;
    b->dirX          = 1;
    b->dirY          = 0;
    b->radius        = DEFAULT_BALL_RADIUS;
    b->color         = WHITE;
    b->velocity      = DEFAULT_BALL_SPEED;
    b->counter       = 0.0f;
    b->alive         = true;
}

