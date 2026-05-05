#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "constants.h"
#include "types.h"
#include "balls.h"


//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
BallTier heldBallTier = One;
BallContainer *ballContainer;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and draw one frame


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

    if (heldBallTier != Zero) {
        const int mouseX = GetMouseX();
        DrawCircle(mouseX, HELD_BALL_Y_POS, CalcBallSize(heldBallTier), BALL_COLORS[heldBallTier]);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            const int ind = AddBall(ballContainer);
            ballContainer->balls[ind].tier = heldBallTier;
            ballContainer->balls[ind].position.x = (float) mouseX;
            ballContainer->balls[ind].position.y = (float) HELD_BALL_Y_POS;
            ballContainer->balls[ind].velocity.y = BALL_INITIAL_SPEED;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            const int ind = AddBall(ballContainer);
            ballContainer->balls[ind].tier = heldBallTier;
            ballContainer->balls[ind].position.x = (float) mouseX;
            ballContainer->balls[ind].position.y = (float) HELD_BALL_Y_POS;
            ballContainer->balls[ind].velocity.y = BALL_INITIAL_SPEED;
        }
        if (IsKeyPressed(KEY_R)) {
            ResetContainer(ballContainer);
        }
    }

    for (int i = 0; i < ballContainer->ballsCapacity; i++) {
        const Ball *b = &ballContainer->balls[i];
        if (b->tier != Zero) {
            DrawCircle((int) roundf(b->position.x), (int) roundf(b->position.y), CalcBallSize(b->tier),
                       BALL_COLORS[b->tier]);
        }
    }
    DrawFPS(10, 10);
    DrawText(TextFormat("%2i balls", ballContainer->ballsCount), 10, 25, 20, BLACK);

    EndDrawing();
    //----------------------------------------------------------------------------------
}


