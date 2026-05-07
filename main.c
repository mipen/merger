#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "constants.h"
#include "types.h"
#include "balls.h"
#include "globals.h"
#include "screen.h"


//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
BallTier heldBallTier = T1;
BallContainer *ballContainer;
bool gameOver = false;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void Update(void);

static void DrawFrame(void);

static void LoadTextures(void);

static void drawSprite(const Texture2D *ss, float x, float y, const Sprite *sprite);

static BallTier GetNextBall(void);

static void SpawnBall(float xPos);

static void drawAllBalls(void);

static Texture2D *spriteSheet;

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    SetTraceLogLevel(LOG_DEBUG);
    const int screenWidth = 400;
    const int screenHeight = 866;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "merger");

    const RenderTexture2D canvas = LoadRenderTexture(BASE_WIDTH, BASE_HEIGHT);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

    LoadTextures();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    ballContainer = CreateContainer(MAX_BALLS);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        RecalculateScreen();
        Update();

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);

        DrawFrame();

        EndTextureMode();

        const Rectangle src = {0, 0, (float) BASE_WIDTH, -(float) BASE_HEIGHT};
        const Rectangle dest = {SCREEN_OFFSETX, SCREEN_OFFSETY, (float) BASE_WIDTH * SCREEN_SCALE, (float) BASE_HEIGHT * SCREEN_SCALE};

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(canvas.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(ballContainer);
    ballContainer = NULL;

    UnloadTexture(*spriteSheet);
    spriteSheet = NULL;
    UnloadRenderTexture(canvas);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


    return 0;
}

static void Update() {
    const float heldBallRadius = GetBallRadius(heldBallTier);
    const float heldBallPosX = fmaxf(fminf((float) GetMouseXRelative(), GetRightWallBound() - heldBallRadius), GetLeftWallBound() + heldBallRadius);

    for (int i = 0; i < ballContainer->ballsCapacity; i++) {
        const Ball *b = &ballContainer->balls[i];
        if (b->tier != None && b->position.y > (float) (BASE_HEIGHT - BOTTOM_BOUND)) {
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
    }
    if (IsKeyPressed(KEY_R)) {
        ResetContainer(ballContainer);
        gameOver = false;
        SCORE = 0;
    }
}

static void DrawFrame(void) {
    const float heldBallRadius = GetBallRadius(heldBallTier);
    const float heldBallPosX = fmaxf(fminf((float) GetMouseXRelative(), GetRightWallBound() - heldBallRadius), GetLeftWallBound() + heldBallRadius);

    const Rectangle rect = (Rectangle){GetLeftWallBound(), GetTopWallBound(), (GetRightWallBound() - GetLeftWallBound()), (GetBottomWallBound() - GetTopWallBound())};
    DrawRectangleRoundedLinesEx(rect, 0.08f, 100, 2.f, GRAY);
    DrawLineEx((Vector2){GetLeftWallBound(), (float) (BASE_HEIGHT - BOTTOM_BOUND)}, (Vector2){GetRightWallBound(), (float) (BASE_HEIGHT - BOTTOM_BOUND)}, 2.f, GRAY);

    // Draw the held ball
    if (!gameOver) {
        // DrawCircle((int) roundf(heldBallPosX), BASE_HEIGHT - BOTTOM_BOUND, heldBallRadius, BALL_COLORS[heldBallTier - 1]);
        const Sprite *sprite = &BALL_SPRITES[heldBallTier - 1];
        drawSprite(spriteSheet, heldBallPosX, (float) (BASE_HEIGHT - BOTTOM_BOUND - 20), sprite);
    }

    const int count = ballContainer->ballsCount;
    if (count > 0) {
        DrawCommand commands[count];
        int j = 0;
        for (int i = 0; i < ballContainer->ballsCapacity; i++) {
            const Ball *b = &ballContainer->balls[i];
            if (b->tier != None) {
                commands[j].ball = b;
                commands[j].sortY = (int) (b->position.y + GetBallRadius(b->tier));
                j++;
            }
        }
        for (int i = 0; i < count; i++) {
            const DrawCommand key = commands[i];
            int jj = i - 1;

            while (jj >= 0 && commands[jj].sortY > key.sortY) {
                commands[jj + 1] = commands[jj];
                jj--;
            }

            commands[jj + 1] = key;
        }
        for (int i = 0; i < count; i++) {
            const Ball *b = commands[i].ball;

            // DrawCircle((int) roundf(b->position.x), (int) roundf(b->position.y), CalcBallRadius(b->tier), BALL_COLORS[b->tier - 1]);
            const Sprite *sprite = &BALL_SPRITES[b->tier - 1];
            drawSprite(spriteSheet, b->position.x, b->position.y, sprite);
        }
    }

    // DrawFPS(10, 10);
    const int fontSize = 20;
    const char *scoreText = TextFormat("Score: %2i", SCORE);
    DrawText(scoreText, 10, 10, fontSize, BLACK);
    const char *btext = TextFormat("%2i balls", ballContainer->ballsCount);
    const int textWidth = MeasureText(btext, fontSize);
    DrawText(btext, (int) GetRightWallBound() - textWidth, 10, fontSize, BLACK);

    if (gameOver) {
        const char *text = "Game over!";
        const int width = MeasureText(text, 40);
        const int posX = (BASE_WIDTH / 2) - (width / 2);
        const int posY = (BASE_HEIGHT / 2) - 20;
        const int margin = 10;
        const Rectangle goRect = (Rectangle){(float) (posX - margin), (float) (posY - margin), (float) (width + margin * 2), (float) (40 + margin * 2)};
        DrawRectangleRounded(goRect, 0.2f, 100, GRAY);
        DrawRectangleRoundedLinesEx(goRect, 0.2f, 100, 2.f, BLACK);
        DrawText(text, posX, posY, 40, BLACK);
    }
}

static void SpawnBall(const float xPos) {
    const int ind = AddBall(ballContainer);
    Ball *b = &ballContainer->balls[ind];
    b->tier = heldBallTier;
    b->position.x = xPos;
    b->position.y = (float) (BASE_HEIGHT - BOTTOM_BOUND - 20);
    b->velocity.y = BALL_INITIAL_SPEED;

    SCORE += heldBallTier;

    heldBallTier = GetNextBall();
}

static BallTier GetNextBall() {
    const int rVal = GetRandomValue(0, 99);
    if (rVal <= 60) return T1;
    if (rVal <= 94) return T2;
    return T3;
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

    // SetTextureFilter(*spriteSheet, TEXTURE_FILTER_POINT);
}

static void drawSprite(const Texture2D *ss, const float x, const float y, const Sprite *sprite) {
    DrawTexturePro(
        *ss,
        sprite->source,
        (Rectangle){x, y, sprite->destWidth, sprite->destHeight},
        sprite->offset,
        0.f,
        WHITE
    );
}

static void drawAllBalls() {
    int prevPos = 50;
    for (int i = 0; i < NUM_BALL_TIERS - 1; i++) {
        const float radius = GetBallRadius(i + 1);
        const int yPos = prevPos + (int) (radius * 2) + 10;
        prevPos = yPos;
        DrawCircle(150, yPos, radius, BALL_COLORS[i]);
        const Sprite *sprite = &BALL_SPRITES[i];
        drawSprite(spriteSheet, 150, (float) yPos, sprite);
    }
}
