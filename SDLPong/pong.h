#pragma once

enum {
	WINDOW_WIDTH = 800,
	WINDOW_HEIGHT = 480
};


extern struct Game* init(SDL_Renderer*);
extern void update(struct Game* game, SDL_Renderer* renderer);
extern void handle_event(struct Game* game, const SDL_Event*);
extern void quit(struct Game* game);
