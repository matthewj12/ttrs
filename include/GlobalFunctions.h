#pragma once

#include "GlobalVariables.h"

#include <fstream>

void processQuitEvents(SDL_Event& p_event);
bool isDirectionKey(kc p_key);
int getRandInt(int p_min, int p_max);
void limitTPS();
void drawBox(SDL_Rect p_rect, int p_width, array<Uint8, 3>* p_color_ptr);
void drawBox(int p_x, int p_y, int p_w, int p_h, int p_width, array<Uint8, 3>* p_color_ptr);
void setFirstLineClearsToAdvance();
void loadData();
void saveData();
