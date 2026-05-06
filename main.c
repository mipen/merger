#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "constants.h"
#include "types.h"
#include "balls.h"


//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
BallTier heldBallTier = T1;
BallContainer *ballContainer;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and draw one frame
static BallTier GetNextBall(void);

static void SpawnBall(float xPos);


//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    SetTraceLogLevel(LOG_DEBUG);
    const int screenWidth = 400;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    ballContainer = CreateContainer(MAX_BALLS);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    free(ballContainer);
    ballContainer = NULL;

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    // Update
    //----------------------------------------------------------------------------------
    //Move balls
    UpdateBalls(ballContainer);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (heldBallTier != None) {
        const float mouseX = (float) GetMouseX();
        const float heldBallSize = CalcBallSize(heldBallTier);
        const float xPos = fmaxf(fminf(mouseX, GetRightWallBound() - heldBallSize), GetLeftWallBound() + heldBallSize);

        DrawCircle((int) roundf(xPos), HELD_BALL_Y_POS, heldBallSize, BALL_COLORS[heldBallTier - 1]);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            SpawnBall(xPos);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            SpawnBall(xPos);
        }
        if (IsKeyPressed(KEY_R)) {
            ResetContainer(ballContainer);
        }
    }

    for (int i = 0; i < ballContainer->ballsCapacity; i++) {
        const Ball *b = &ballContainer->balls[i];
        if (b->tier != None) {
            DrawCircle((int) roundf(b->position.x), (int) roundf(b->position.y), CalcBallSize(b->tier),
                       BALL_COLORS[b->tier - 1]);
        }
    }
    DrawFPS(10, 10);
    DrawText(TextFormat("%2i balls", ballContainer->ballsCount), 10, 25, 20, BLACK);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

static void SpawnBall(const float xPos) {
    const int ind = AddBall(ballContainer);
    Ball *b = &ballContainer->balls[ind];
    b->tier = heldBallTier;
    b->radius = CalcBallSize(heldBallTier);
    b->position.x = xPos;
    b->position.y = (float) HELD_BALL_Y_POS;
    b->velocity.y = BALL_INITIAL_SPEED;

    heldBallTier = GetNextBall();
}

static BallTier GetNextBall() {
    const int rVal = GetRandomValue(0, 9);
    if (rVal <= 4) return T1;
    if (rVal <= 7) return T2;
    return T3;
}
