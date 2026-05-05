#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef enum {
    Zero = 0,
    One = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5
} BallTier;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    BallTier tier;
    int nextFree;
    bool hitWall;
} Ball;

typedef struct {
    int firstFreeBall;
    int ballsCapacity;
    int ballsCount;
    Ball balls[];
} BallContainer;

#endif
