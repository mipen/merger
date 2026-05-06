#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "constants.h"
#include "types.h"
#include "balls.h"
#include "globals.h"


//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
BallTier heldBallTier = T1;
BallContainer *ballContainer;
bool gameOver = false;

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
    // ---- Update ----
    const float mouseX = (float) GetMouseX();
    const float heldBallSize = CalcBallSize(heldBallTier);
    const float heldBallPosX = fmaxf(fminf(mouseX, GetRightWallBound() - heldBallSize), GetLeftWallBound() + heldBallSize);

    for (int i = 0; i < ballContainer->ballsCapacity; i++) {
        const Ball *b = &ballContainer->balls[i];
        if (b->tier != None && b->position.y > (float) BOTTOM_BOUND) {
            gameOver = true;
            break;
        }
    }

    if (!gameOver) {
        // Move balls
        UpdateBalls(ballContainer);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            SpawnBall(heldBallPosX);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            SpawnBall(heldBallPosX);
        }
        if (IsKeyPressed(KEY_R)) {
            ResetContainer(ballContainer);
        }
    }

    // ---- Draw ----
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw the held ball
    if (!gameOver) {
        DrawCircle((int) roundf(heldBallPosX), BOTTOM_BOUND, heldBallSize, BALL_COLORS[heldBallTier - 1]);
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
    DrawText(TextFormat("Score: %2i", SCORE), GetScreenWidth() - 150, 10, 20, BLACK);

    if (gameOver) {
        DrawText("Game over!", 100, 380, 40,BLACK);
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

static void SpawnBall(const float xPos) {
    const int ind = AddBall(ballContainer);
    Ball *b = &ballContainer->balls[ind];
    b->tier = heldBallTier;
    b->radius = CalcBallSize(heldBallTier);
    b->position.x = xPos;
    b->position.y = (float) BOTTOM_BOUND;
    b->velocity.y = BALL_INITIAL_SPEED;

    SCORE += heldBallTier;

    heldBallTier = GetNextBall();
}

static BallTier GetNextBall() {
    const int rVal = GetRandomValue(0, 10);
    if (rVal <= 4) return T1;
    if (rVal <= 7) return T2;
    if (rVal <= 9) return T3;
    return T4;
}
