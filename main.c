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
static void LoadTextures(void);

static void drawSprite(const Texture2D *ss, Rectangle source, Rectangle dest);

static BallTier GetNextBall(void);

static void SpawnBall(float xPos);

static Texture2D *spriteSheet;

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    SetTraceLogLevel(LOG_DEBUG);
    const int screenWidth = 400;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "merger");

    LoadTextures();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    ballContainer = CreateContainer(MAX_BALLS);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
        for (int i = 0; i < NUM_BALL_TIERS; i++) {
            const int yPos = 100 * (i + 1);
            float radius = CalcBallRadius(i + 1);
            DrawCircle(200, yPos, radius, BALL_COLORS[heldBallTier - 1]);
            const Sprite *sprite = &BALL_SPRITES[i];
            drawSprite(spriteSheet, sprite->source, (Rectangle){.x = 200, .y = (float) yPos, .width = sprite->destWidth.height = sprite->destHeight});
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    free(ballContainer);
    ballContainer = NULL;
    free(spriteSheet);
    spriteSheet = NULL;

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    // ---- Update ----
    const float mouseX = (float) GetMouseX();
    const float heldBallRadius = CalcBallRadius(heldBallTier);
    const float heldBallPosX = fmaxf(fminf(mouseX, GetRightWallBound() - heldBallRadius), GetLeftWallBound() + heldBallRadius);

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
        DrawCircle((int) roundf(heldBallPosX), BOTTOM_BOUND, heldBallRadius, BALL_COLORS[heldBallTier - 1]);
        const Sprite *sprite = &BALL_SPRITES[heldBallTier - 1];
        drawSprite(spriteSheet, sprite->source, (Rectangle){.x = heldBallPosX, .y = (float) BOTTOM_BOUND, .width = sprite->destWidth, .height = sprite->destHeight});
    }

    for (int i = 0; i < ballContainer->ballsCapacity; i++) {
        const Ball *b = &ballContainer->balls[i];
        if (b->tier != None) {
            // DrawCircle((int) roundf(b->position.x), (int) roundf(b->position.y), CalcBallRadius(b->tier), BALL_COLORS[b->tier - 1]);
            const Sprite *sprite = &BALL_SPRITES[b->tier - 1];
            drawSprite(spriteSheet, sprite->source, (Rectangle){.x = b->position.x, .y = b->position.y, .width = sprite->destWidth, .height = sprite->destHeight});
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
    b->radius = CalcBallRadius(heldBallTier);
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

static void LoadTextures() {
    // textures = malloc(sizeof(Texture2D) * NUM_BALL_TIERS);
    //
    // textures[0] = LoadTexture("./assets/Copper Ingot.png");
    // textures[1] = LoadTexture("./assets/Silver Ingot.png");
    // textures[2] = LoadTexture("./assets/Golden Ingot.png");
    // textures[3] = LoadTexture("./assets/Cut Sapphire.png");
    // textures[4] = LoadTexture("./assets/Cut Emerald.png");
    // textures[5] = LoadTexture("./assets/Cut Ruby.png");
    // textures[6] = LoadTexture("./assets/Obsidian.png");
    // textures[7] = LoadTexture("./assets/Crystal.png");
    // textures[8] = LoadTexture("./assets/Diamond.png");

    spriteSheet = malloc(sizeof(Texture2D));
    *spriteSheet = LoadTexture("./assets/icons-1.png");

    SetTextureFilter(*spriteSheet, TEXTURE_FILTER_POINT);
}

static void drawSprite(const Texture2D *ss, const Rectangle source, const Rectangle dest) {
    DrawTexturePro(
        *ss,
        source,
        dest,
        (Vector2){.x = dest.width * 0.5f, .y = dest.height * 0.5f},
        0.f,
        WHITE
    );
}
