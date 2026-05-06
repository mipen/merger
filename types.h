#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef enum {
    None = 0,
    T1 = 1,
    T2 = 2,
    T3 = 3,
    T4 = 4,
    T5 = 5,
    T6 = 6,
    T7 = 7,
    T8 = 8,
    T9 = 9,
} BallTier;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    BallTier tier;
    float radius;
    short nextFree;
} Ball;

typedef struct {
    int maxBalls;
    int ballsCapacity;
    int ballsCount;
    short firstFreeBall;
    Ball balls[];
} BallContainer;

#endif
