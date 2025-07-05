#pragma once
extern bool drawInit(SDL_Renderer* renderer);
extern void drawQuit(void);
extern void drawImage(SDL_Renderer* renderer, const char* image, float x, float y, bool center);
