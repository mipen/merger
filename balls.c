#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "balls.h"

#include "constants.h"

static void resolveWallCollisions(Ball *b, float leftWall, float rightWall, float topWall, float bottomWall, Vector2 *newVel);

static void resolveBallCollisions(BallContainer *bc, Ball *bi, Vector2 *newVel, int i, bool *removed);

BallContainer *CreateContainer(const int maxBalls) {
    BallContainer *bc = malloc(sizeof(BallContainer) + maxBalls * sizeof(Ball));
    if (!bc) return NULL;

    bc->maxBalls = maxBalls;
    ResetContainer(bc);

    return bc;
}

void UpdateBalls(BallContainer *bc) {
    const float dt = GetFrameTime();
    const float leftWallBound = GetLeftWallBound();
    const float rightWallBound = GetRightWallBound();
    const float topWallBound = GetTopWallBound();
    const float bottomWallBound = (float) GetScreenHeight();

    for (int i = 0; i < bc->ballsCapacity; i++) {
        Ball *bi = &bc->balls[i];
        if (bi->tier == None) continue;

        //Friction slow down
        Vector2 newVel = Vector2Scale(bi->velocity, expf(-FRICTION * dt));


        //Ball-Ball collisions
        bool ballRemoved = false;
        resolveBallCollisions(bc, bi, &newVel, i, &ballRemoved);
        //Wall collisions
        resolveWallCollisions(bi, leftWallBound, rightWallBound, topWallBound, bottomWallBound, &newVel);

        if (!ballRemoved) {
            if (Vector2LengthSqr(newVel) > MIN_SPEED_SQ) {
                bi->velocity = newVel;
                bi->position = Vector2Add(bi->position, Vector2Scale(bi->velocity, GetFrameTime()));
            } else {
                bi->velocity.x = 0.f;
                bi->velocity.y = 0.f;
            }
        }
    }
}

static void resolveBallCollisions(BallContainer *bc, Ball *bi, Vector2 *newVel, const int i, bool *removed) {
    const float biRadius = bi->radius;

    for (int j = i + 1; j < bc->ballsCapacity; j++) {
        Ball *bj = &bc->balls[j];
        if (bj->tier == None) continue;

        const Vector2 distVec = Vector2Subtract(bj->position, bi->position);
        const float distSq = Vector2LengthSqr(distVec);
        float radiusSq = biRadius + bj->radius;
        radiusSq = radiusSq * radiusSq;

        if (distSq <= radiusSq) {
            if (bi->tier == bj->tier && bi->tier < MAX_BALL_TIER) {
                const BallTier newTier = GetNextTier(bj->tier);
                bj->tier = newTier;
                bj->radius = CalcBallSize(newTier);
                bj->position = Vector2Scale(Vector2Add(bi->position, bj->position), 0.5f);
                bj->velocity = Vector2Scale(Vector2Add(*newVel, bj->velocity), 0.5f);
                RemoveBall(bc, i);
                *removed = true;
                break;
            }

            const float dist = sqrtf(distSq);

            // Avoid divide-by-zero if perfectly overlapping
            Vector2 collisionNormal;
            if (dist > 0.0001f) {
                collisionNormal = Vector2Scale(distVec, 1.0f / dist);
            } else {
                // arbitrary direction if on top of each other
                collisionNormal = Vector2One();
            }

            const float penetration = (biRadius + bj->radius) - dist;

            // --- POSITION CORRECTION ---
            const float percent = 0.2f; // usually 0.2–0.8
            const float slop = 0.01f; // small tolerance
            const float mi_inv = 1.f / CalcBallMass(bi->tier);
            const float mj_inv = 1.f / CalcBallMass(bj->tier);

            const float correctionMag = fmaxf(penetration - slop, 0.0f) / (mi_inv + mj_inv);
            const Vector2 correction = Vector2Scale(collisionNormal, correctionMag * percent);

            bj->position = Vector2Add(bj->position, Vector2Scale(correction, mj_inv));

            const Vector2 relativeVelocity = Vector2Subtract(bj->velocity, *newVel);
            const float velAlongNormal = Vector2DotProduct(relativeVelocity, collisionNormal);

            if (velAlongNormal < 0.f) {
                float impulseForce = -(1 + BALL_RESTITUTION) * velAlongNormal;
                impulseForce /= mi_inv + mj_inv;
                const Vector2 impulse = Vector2Scale(collisionNormal, impulseForce);
                *newVel = Vector2Subtract(*newVel, Vector2Scale(impulse, mi_inv));
                bj->velocity = Vector2Add(bj->velocity, Vector2Scale(impulse, mj_inv));
            }
        }
    }
}

static void resolveWallCollisions(Ball *b, const float leftWall, const float rightWall, const float topWall, const float bottomWall, Vector2 *newVel) {
    const float radius = b->radius;

    if (b->position.x - radius < leftWall) {
        b->position.x = leftWall + radius;
        if (newVel->x < 0.f) {
            newVel->x = -newVel->x * SIDE_WALL_RESTITUTION;
        }
    } else if (b->position.x + radius > rightWall) {
        b->position.x = rightWall - radius;
        if (newVel->x > 0.f) {
            newVel->x = -newVel->x * SIDE_WALL_RESTITUTION;
        }
    }

    if (b->position.y - radius < topWall) {
        b->position.y = topWall + radius;
        if (newVel->y < 0.f) {
            newVel->y = -newVel->y * TOP_WALL_RESTITUTION;
        }
    } else if (b->position.y + radius > bottomWall) {
        b->position.y = bottomWall - radius;
        if (newVel->y > 0.f) {
            newVel->y = -newVel->y * TOP_WALL_RESTITUTION;
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
        bc->firstFreeBall = (short) ind;
        ResetBall(bc, ind);
    } else {
        const short prevFirstFreeBall = bc->firstFreeBall;
        bc->firstFreeBall = (short) ind;
        ResetBall(bc, ind);
        bc->balls[ind].nextFree = prevFirstFreeBall;
    }
    bc->ballsCount--;
}

int GetNextTier(const int tier) {
    if (tier < MAX_BALL_TIER)
        return tier + 1;
    return tier;
}

void ResetBall(BallContainer *bc, const int ind) {
    Ball *ball = &bc->balls[ind];
    ball->tier = None;
    ball->position.x = 0;
    ball->position.y = 0;
    ball->velocity.x = 0;
    ball->velocity.y = 0;
    ball->nextFree = -1;
    ball->radius = 0.f;
}

void ResetContainer(BallContainer *bc) {
    TraceLog(LOG_DEBUG, "Resetting container");
    bc->firstFreeBall = -1;
    bc->ballsCount = 0;
    bc->ballsCapacity = 0;

    for (int i = 0; i < bc->maxBalls; i++) {
        ResetBall(bc, i);
    }
}

float CalcBallSize(const int tier) {
    return (float) (10 + pow(1.6f, tier));
}

float CalcBallMass(const int tier) {
    return 10.f + (float) (tier * tier * tier);
}

float GetLeftWallBound() {
    return 20.f;
}

float GetRightWallBound() {
    return (float) GetScreenWidth() - 20.f;
}

float GetTopWallBound() {
    return 100.f;
}
