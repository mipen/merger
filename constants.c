#include <raylib.h>
#include "constants.h"

#include "types.h"

//Ball Stuff
const int MAX_BALLS = 1000;
const float MIN_SPEED = 2.8f;
const float MIN_SPEED_SQ = MIN_SPEED * MIN_SPEED;
const float BALL_INITIAL_SPEED = -600.f;
const BallTier MAX_BALL_TIER = T9;

//Physics
const float BALL_RESTITUTION = 0.25f;
const float RESTING_THRESHOLD = 5.f;
const float SIDE_WALL_RESTITUTION = 0.5f;
const float TOP_WALL_RESTITUTION = 0.05f;
const float FRICTION = 0.97f;

//Game
const int BOTTOM_BOUND = 100;

const Color BALL_COLORS[NUM_BALL_TIERS] = {
    BLUE, YELLOW, GREEN, ORANGE, PURPLE, RED, DARKBLUE,
    // BEIGE, GOLD
};
const float BALL_RADII[NUM_BALL_TIERS] = {
    12, 16, 24, 30, 40, 50, 60,
    // 50, 60
};
const Sprite BALL_SPRITES[NUM_BALL_TIERS] = {
    (Sprite){.source = (Rectangle){.x = 0, .y = 32, .width = 32, .height = 32}, .offset = (Vector2){17, 22}, .destWidth = 34.f, .destHeight = 34.f}, //T1
    (Sprite){.source = (Rectangle){.x = 10 * 32 + 12, .y = 0, .width = 32, .height = 32}, .offset = (Vector2){42 / 2, 27}, .destWidth = 42.f, .destHeight = 42.f}, //T2
    (Sprite){.source = (Rectangle){.x = 9 * 32 + 11, .y = 0, .width = 32, .height = 32}, .offset = (Vector2){70 / 2, 44}, .destWidth = 70.f, .destHeight = 70.f}, //T3
    (Sprite){.source = (Rectangle){.x = 9 * 32 + 11, .y = 32, .width = 32, .height = 32}, .offset = (Vector2){82 / 2, 48}, .destWidth = 82.f, .destHeight = 82.f}, //T4
    (Sprite){.source = (Rectangle){.x = 5 * 32 + 7, .y = 0, .width = 32, .height = 32}, .offset = (Vector2){112 / 2, 70}, .destWidth = 112.f, .destHeight = 112.f}, //T5
    (Sprite){.source = (Rectangle){.x = 4 * 32 + 5, .y = 0, .width = 32, .height = 32}, .offset = (Vector2){140 / 2, 86}, .destWidth = 140.f, .destHeight = 140.f}, //T6
    (Sprite){.source = (Rectangle){.x = 6 * 32 + 8, .y = 32, .width = 32, .height = 32}, .offset = (Vector2){184 / 2, 118}, .destWidth = 184.f, .destHeight = 184.f}, //T7
    // (Sprite){.source = (Rectangle){.x = 8 * 32 + 9, .y = 0, .width = 32, .height = 32}, .offset = (Vector2){16, 0}, .destWidth = 32.f, .destHeight = 32.f}, //T8
    // (Sprite){.source = (Rectangle){.x = 7 * 32 + 8, .y = 32, .width = 32, .height = 32}, .offset = (Vector2){16, 0}, .destWidth = 32.f, .destHeight = 32.f}, //T9
};
