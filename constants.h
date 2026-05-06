#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "types.h"

//Ball Stuff
#define NUM_BALL_TIERS 9
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
extern const int BOTTOM_BOUND;


extern const Color BALL_COLORS[NUM_BALL_TIERS];
extern const Sprite BALL_SPRITES[NUM_BALL_TIERS];

#endif
