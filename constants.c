#include <raylib.h>
#include "constants.h"

#include "types.h"

//Ball Stuff
const int MAX_BALLS = 1000;
const float MIN_SPEED = 2.8f;
const float MIN_SPEED_SQ = MIN_SPEED * MIN_SPEED;
const float BALL_INITIAL_SPEED = -1500.f;
const BallTier MAX_BALL_TIER = T9;

//Physics
const float BALL_RESTITUTION = 0.45f;
const float SIDE_WALL_RESTITUTION = 0.5f;
const float TOP_WALL_RESTITUTION = 0.1f;
const float FRICTION = 0.99f;

//Game
const int BOTTOM_BOUND = 700;

const Color BALL_COLORS[NUM_BALL_TIERS] = {
    BLUE, YELLOW, GREEN, ORANGE, PURPLE, RED, DARKBLUE, BEIGE, GOLD
};
const Sprite BALL_SPRITES[NUM_BALL_TIERS] = {
    (Sprite){.source = (Rectangle){.x = 0, .y = 32, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T1
    (Sprite){.source = (Rectangle){.x = 10 * 32 + 12, .y = 0, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T2
    (Sprite){.source = (Rectangle){.x = 9 * 32 + 11, .y = 0, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T3
    (Sprite){.source = (Rectangle){.x = 9 * 32 + 11, .y = 32, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T4
    (Sprite){.source = (Rectangle){.x = 5 * 32 + 6, .y = 0, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T5
    (Sprite){.source = (Rectangle){.x = 4 * 32 + 5, .y = 0, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T6
    (Sprite){.source = (Rectangle){.x = 6 * 32 + 7, .y = 32, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T7
    (Sprite){.source = (Rectangle){.x = 8 * 32 + 9, .y = 0, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T8
    (Sprite){.source = (Rectangle){.x = 7 * 32 + 8, .y = 32, .width = 32, .height = 32}, .destWidth = 32.f, .destHeight = 32.f}, //T9
};
