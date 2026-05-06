#pragma once

#include "types.h"

#ifndef BALL_CONTAINER_H
#define BALL_CONTAINER_H

BallContainer *CreateContainer(int maxBalls);

void UpdateBalls(BallContainer *bc);

int AddBall(BallContainer *bc);

void RemoveBall(BallContainer *bc, int ind);

int GetNextTier(int tier);

void ResetBall(BallContainer *bc, int ind);

void ResetContainer(BallContainer *bc);

float CalcBallSize(int tier);

float CalcBallMass(int tier);

float GetLeftWallBound();

float GetRightWallBound();

float GetTopWallBound();

#endif
