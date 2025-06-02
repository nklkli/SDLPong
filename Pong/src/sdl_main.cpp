#define SDL_MAIN_USE_CALLBACKS /* use the callbacks instead of main() */
#include <SDL_mixer.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
import std;
import Pong;
import SDLEngine;
import Game;
import Input;
import SDLEventToInput;
using namespace std;

struct AppState
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	float gameUpdateStepSecs = 10 / 1000.0f;
	unique_ptr<Game> game{nullptr};
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	try
	{
		auto app = new AppState();
		*appstate = app;

		SDL_SetLogPriorities(SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE);

		SDL_SetAppMetadata("Example Renderer Clear",
			"1.0",
			"com.example.renderer-clear");

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			throw format("SDL_INIT_VIDEO SDL:\n{}", SDL_GetError());
		}

	
		if (!SDL_CreateWindowAndRenderer(
			"examples/renderer/clear",
		   	GamePong::WIDTH,
			GamePong::HEIGHT,
			0,
			&app->window,
			&app->renderer))
		{
			throw format("SDL_CreateWindowAndRenderer:\n{}", SDL_GetError());
		}

		if (!SDL_SetRenderVSync(app->renderer, 1))
		{
			throw format("SDL_SetRenderVSync failed:\n{}", SDL_GetError());
		}

		auto images_folder = format("{}{}", SDL_GetBasePath(),
			"assets/images");
		auto sound_folder = format("{}{}", SDL_GetBasePath(), 
			"assets/sounds");
		unique_ptr<Engine> engine = make_unique<EngineSDL>(
			app->renderer, 
			images_folder,
			sound_folder);
	    app->game.reset(
			new GamePong(move(engine))
		);

	}
	catch (const std::string& err)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", err.c_str(), nullptr);
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc.) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT ||
		(event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE))
	{
		return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
	}

	AppState& app = *static_cast<AppState*>(appstate);

	app.game->handleInput(mapSDLEventToInput(event));

	return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
	AppState& appState = *static_cast<AppState*>(appstate);

	static Uint64 last_ticks = 0;
	const auto current_ticks = SDL_GetTicks();
	const auto elapsed_milliseconds = current_ticks - last_ticks; // how many milliseconds elapsed since last frame?
	last_ticks = current_ticks;
	const auto elapsed_secs = elapsed_milliseconds / 1000.0f;
	auto lag_secs = elapsed_secs;

	while (lag_secs > 0) {
		appState.game->update( appState.gameUpdateStepSecs);
		lag_secs -= appState.gameUpdateStepSecs;
		lag_secs = SDL_max(0, lag_secs);
	}

	SDL_SetRenderDrawColor(appState.renderer, 230, 230, 230, SDL_ALPHA_OPAQUE);

	SDL_RenderClear(appState.renderer);

	appState.game->draw();

	SDL_RenderPresent(appState.renderer);

	return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	AppState* app = static_cast<AppState*>(appstate);	
	delete app;
	SDL_Quit();
	/* SDL will clean up the window/renderer for us. */
}