#include <raylib.h>
#include <raymath.h>

#include "screen.h"

const int BASE_WIDTH = 300;
const int BASE_HEIGHT = 650; //9:19.5 aspect ratio

float SCREEN_SCALE;
float SCREEN_SCALE_INV;
float SCREEN_OFFSETX;
float SCREEN_OFFSETY;

void RecalculateScreen() {
    SCREEN_SCALE = fminf((float) GetScreenWidth() / (float) BASE_WIDTH, (float) GetScreenHeight() / (float) BASE_HEIGHT);
    SCREEN_SCALE_INV = 1.0f / SCREEN_SCALE;
    SCREEN_OFFSETX = ((float) GetScreenWidth() - (float) BASE_WIDTH * SCREEN_SCALE) * 0.5f;
    SCREEN_OFFSETY = ((float) GetScreenHeight() - (float) BASE_HEIGHT * SCREEN_SCALE) * 0.5f;
}

int GetMouseXRelative() {
    return ((float) GetMouseX() - SCREEN_OFFSETX) * SCREEN_SCALE_INV;
}

int GetMouseYRelative() {
    return ((float) GetMouseY() - SCREEN_OFFSETY) * SCREEN_SCALE_INV;
}
