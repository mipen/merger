#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "types.h"

//Ball Stuff
extern const int MAX_BALLS;
extern const float MIN_SPEED;
extern const float MIN_SPEED_SQ;
extern const float BALL_INITIAL_SPEED;
extern const BallTier MAX_BALL_TIER;

//Physics
extern const float BALL_RESTITUTION;
extern const float SIDE_WALL_RESTITUTION;
extern const float TOP_WALL_RESTITUTION;
extern const float FRICTION;

//Game
extern const int HELD_BALL_Y_POS;


extern const Color BALL_COLORS[9];

#endif
