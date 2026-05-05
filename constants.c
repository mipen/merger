#include <raylib.h>
#include "constants.h"

const int MAX_BALLS = 1000;
const float MIN_SPEED = 2.f;
const float MIN_SPEED_SQ = MIN_SPEED * MIN_SPEED;
const float BALL_RESTITUTION = 0.95f;
// const float WALL_RESTITUTION = 1.f;
const float WALL_RESTITUTION = 0.9f;
// const float FRICTION = 1.f;
const float FRICTION = 0.99f;
const int HELD_BALL_Y_POS = 700;
const float BALL_INITIAL_SPEED = -1500.f;
const Color BALL_COLORS[6] = {
    RED, BLUE, YELLOW, GREEN, ORANGE, PURPLE
};
