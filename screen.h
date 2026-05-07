#pragma once

#ifndef MERGER_SCREEN_H
#define MERGER_SCREEN_H

extern const int BASE_WIDTH;
extern const int BASE_HEIGHT;

extern float SCREEN_SCALE;
extern float SCREEN_SCALE_INV;
extern float SCREEN_OFFSETX;
extern float SCREEN_OFFSETY;

void RecalculateScreen();
int GetMouseXRelative();
int GetMouseYRelative();

#endif //MERGER_SCREEN_H
