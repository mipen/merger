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
const int HELD_BALL_Y_POS = 700;

const Color BALL_COLORS[9] = {
    BLUE, YELLOW, GREEN, ORANGE, PURPLE, RED, DARKBLUE, BEIGE, GOLD
};
