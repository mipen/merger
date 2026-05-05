#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "balls.h"

#include "constants.h"

BallContainer *CreateContainer(const int maxBalls) {
    BallContainer *bc = malloc(sizeof(BallContainer) + maxBalls * sizeof(Ball));
    if (!bc) return NULL;

    bc->firstFreeBall = -1;
    bc->ballsCount = 0;
    bc->ballsCapacity = 0;

    for (int i = 0; i < maxBalls; i++) {
        ResetBall(bc, i);
    }

    return bc;
}

void UpdateBalls(BallContainer *bc) {
    for (int i = 0; i < bc->ballsCapacity; i++) {
        Ball bi = bc->balls[i];
        if (bi.tier == Zero) continue;

        //Friction slow down
        Vector2 newVel = Vector2Scale(bi.velocity, FRICTION);

        //Wall collisions
        const float biSize = CalcBallSize(bi.tier);
        bool applyRestitution = false;
        if (!bi.hitWall && ((bi.position.x - biSize) < 0.f || (bi.position.x + biSize) > (float)
                            GetScreenWidth())) {
            newVel.x *= -1.f;
            bi.hitWall = true;
            applyRestitution = true;
        } else if (!bi.hitWall && ((bi.position.y - biSize) < 0.f || (bi.position.y + biSize) > (float)
                                   GetScreenHeight())) {
            newVel.y *= -1.f;
            bi.hitWall = true;
            applyRestitution = true;
        } else
            bi.hitWall = false;
        if (applyRestitution)
            newVel = Vector2Scale(newVel, WALL_RESTITUTION);

        //Ball-Ball collisions
        bool ballRemoved = false;
        for (int j = i + 1; j < bc->ballsCapacity; j++) {
            const Ball bj = bc->balls[j];
            if (bj.tier == Zero) continue;
            const Vector2 distVec = Vector2Subtract(bj.position, bi.position);
            const float distSq = Vector2LengthSqr(distVec);
            const float bjSize = CalcBallSize(bj.tier);
            float sizeSq = biSize + bjSize;
            sizeSq = sizeSq * sizeSq;
            if (distSq <= sizeSq) {
                if (bi.tier == bj.tier && bi.tier < Five) {
                    bc->balls[j].tier = GetNextTier(bj.tier);
                    bc->balls[j].position = Vector2Scale(Vector2Add(bi.position, bj.position), 0.5f);
                    bc->balls[j].velocity = Vector2Scale(Vector2Add(newVel, bj.velocity), 0.5f);
                    RemoveBall(bc, i);
                    ballRemoved = true;
                    break;
                }
                const Vector2 collisionNormal = Vector2Normalize(distVec);
                const Vector2 relativeVelocity = Vector2Subtract(bj.velocity, newVel);
                const float velAlongNormal = Vector2DotProduct(relativeVelocity, collisionNormal);
                if (velAlongNormal < 0.f) {
                    const float mi_inv = 1.f / (float) (bi.tier * bi.tier * bi.tier);
                    const float mj_inv = 1.f / (float) (bj.tier * bj.tier * bj.tier);
                    float impulseForce = -(1 + BALL_RESTITUTION) * velAlongNormal;
                    impulseForce /= mi_inv + mj_inv;
                    const Vector2 impulse = Vector2Scale(collisionNormal, impulseForce);
                    newVel = Vector2Subtract(newVel, Vector2Scale(impulse, mi_inv));
                    const Vector2 bjNewVel = Vector2Add(bj.velocity, Vector2Scale(impulse, mj_inv));
                    bc->balls[j].velocity = bjNewVel;
                }
            }
        }

        if (!ballRemoved) {
            if (Vector2LengthSqr(newVel) > MIN_SPEED_SQ) {
                bi.velocity = newVel;
                bi.position = Vector2Add(bi.position, Vector2Scale(bi.velocity, GetFrameTime()));
            }
            bc->balls[i] = bi;
        }
    }
}

int AddBall(BallContainer *bc) {
    bc->ballsCount++;
    if (bc->firstFreeBall == -1) {
        //There are no slots which have been freed. Add to end of list
        const int ind = bc->ballsCapacity;
        bc->ballsCapacity += 1;
        return ind;
    }
    const int index = bc->firstFreeBall;
    bc->firstFreeBall = bc->balls[index].nextFree;
    return index;
}

void RemoveBall(BallContainer *bc, const int ind) {
    if (bc->firstFreeBall == -1) {
        bc->firstFreeBall = ind;
        ResetBall(bc, ind);
    } else {
        const int prevFirstFreeBall = bc->firstFreeBall;
        bc->firstFreeBall = ind;
        ResetBall(bc, ind);
        bc->balls[ind].nextFree = prevFirstFreeBall;
    }
    bc->ballsCount--;
}

int GetNextTier(const int tier) {
    if (tier < Five)
        return tier + 1;
    return tier;
}

void ResetBall(BallContainer *bc, const int ind) {
    Ball *ball = &bc->balls[ind];
    ball->tier = Zero;
    ball->position.x = 0;
    ball->position.y = 0;
    ball->velocity.x = 0;
    ball->velocity.y = 0;
    ball->nextFree = -1;
    ball->hitWall = false;
}

float CalcBallSize(const int tier) {
    return (float) (10 + pow(2.f, tier));
}
