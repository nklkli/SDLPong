#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include "pong.h"


/* We will use this renderer to draw into this window every frame. */

struct AppState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	struct Game* game;
};


/* This function runs once at startup. */
#pragma warning (disable: 4100)
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
#pragma warning (default: 4100)


	SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

	SDL_SetAppMetadata("Example Renderer Debug Texture", "1.0", "com.example.renderer-debug-text");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	struct AppState* appState = malloc(sizeof(struct AppState));

	*appstate = appState;

	if (!SDL_CreateWindowAndRenderer("examples/renderer/debug-text",
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0,
		&appState->window,
		&appState->renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		goto  cleanup1;
	}

	if (!SDL_SetRenderVSync(appState->renderer, 1))
	{
		SDL_Log("SDL_SetRenderVSync failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		return SDL_APP_FAILURE;
	}

	appState->game = init(appState->renderer);

	if (!appState->game)
	{
		SDL_Log("Couldn't init game. Program exits");
		goto cleanup2;
	}

	return SDL_APP_CONTINUE;  /* carry on with the program! */

cleanup2:
	free(appState->game);

cleanup1:
	free(appState);

	return SDL_APP_FAILURE;

}



/* This function runs when a new handle_event (mouse input, keypresses, etc) occurs. */
#pragma warning (disable: 4100)
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
#pragma warning (default: 4100)
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}
	struct Game* game = ((struct AppState*)appstate)->game;
	handle_event(game, event);  /* let the app handle the handle_event. */
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}



/* This function runs once per frame, and is the heart of the program. */
#pragma warning (disable: 4100)
SDL_AppResult SDL_AppIterate(void* appstate)
{
#pragma warning (default: 4100)

	struct AppState* appState = ((struct AppState*)appstate);
	
	update(appState->game, appState->renderer);

	//const int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

	///* as you can see from this, rendering draws over whatever was drawn before it. */
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
	//SDL_RenderClear(renderer);  /* start with a blank canvas. */

	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
	//SDL_RenderDebugText(renderer, 272, 100, "Hello world!");
	//SDL_RenderDebugText(renderer, 224, 150, "This is some debug text.");

	//SDL_SetRenderDrawColor(renderer, 51, 102, 255, SDL_ALPHA_OPAQUE);  /* light blue, full alpha */
	//SDL_RenderDebugText(renderer, 184, 200, "You can do it in different colors.");
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */

	//SDL_SetRenderScale(renderer, 4.0f, 4.0f);
	//SDL_RenderDebugText(renderer, 14, 65, "It can be scaled.");
	//SDL_SetRenderScale(renderer, 1.0f, 1.0f);
	//SDL_RenderDebugText(renderer, 64, 350, "This only does ASCII chars. So this laughing emoji won't draw: 🤣");

	//SDL_RenderDebugTextFormat(renderer, (float)((WINDOW_WIDTH - (charsize * 46)) / 2), 400, "(This program has been running for %" SDL_PRIu64 " seconds.)", SDL_GetTicks() / 1000);

	//SDL_RenderPresent(renderer);  /* put it all on the screen! */

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}



/* This function runs once at shutdown. */
#pragma warning (disable: 4100)
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
#pragma warning (default: 4100)
	struct Game* game = ((struct AppState*)appstate)->game;
	quit(game);
	free(appstate);
	/* SDL will clean up the window/renderer for us. */
}



