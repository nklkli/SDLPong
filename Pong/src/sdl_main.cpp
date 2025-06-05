module;
#define SDL_MAIN_USE_CALLBACKS /* use the callbacks instead of main() */
#include <SDL_mixer.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
export module Main;
import SDLGameAdapter;
using namespace std;


SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void**, int argc, char* argv[])
{
	try
	{
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
			0,
			0,
			0,
			&window,
			&renderer))
		{
			throw format("SDL_CreateWindowAndRenderer:\n{}", SDL_GetError());
		}

		if (!SDL_SetRenderVSync(renderer, 1))
		{
			throw format("SDL_SetRenderVSync failed:\n{}", SDL_GetError());
		}

		auto images_folder = format("{}{}", SDL_GetBasePath(),
			"assets/images");
		auto sound_folder = format("{}{}", SDL_GetBasePath(),
			"assets/sounds");


		SDLGameAdapter::init(renderer,
			images_folder,
			sound_folder);

		SDL_SetWindowSize(window,
			SDLGameAdapter::Get_WINDOWS_WIDTH(), SDLGameAdapter::Get_WINDOWS_HEIGHT());
	}
	catch (const std::string& err)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", err.c_str(), nullptr);
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc.) occurs. */
SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT ||
		(event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE))
	{
		return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
	}
	SDLGameAdapter::handleInput(event);
	return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void*)
{
	SDLGameAdapter::update();
	SDL_SetRenderDrawColor(renderer, 230, 230, 230, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDLGameAdapter::draw();
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	SDLGameAdapter::quit();
	SDL_Quit();
	/* SDL will clean up the window/renderer for us. */
}