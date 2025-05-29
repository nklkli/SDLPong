#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
import std;
import Pong;
import EngineSDL;
import Game;
using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
auto gameUpdateStepSecs = 10 / 1000.0f;
GamePong game;
unique_ptr<Engine> engine(nullptr);
Input input = {};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{

	SDL_SetLogPriorities(SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE);
	SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, 0, &window, &renderer))
	{
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderVSync(renderer, 1);

	try
	{
		engine.reset(new EngineSDL{ renderer, "assets/images" });
	}
	catch (const std::string& err)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", err.c_str(), window);
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE; /* carry on with the program! */
}




/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT ||
		(event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE))
	{
		return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
	}
		
	switch (event->type)
	{
	case SDL_EVENT_MOUSE_WHEEL:
		input.MouseWheelY = event->wheel.y;
		break;
	default:
		break;
	}
	

	return SDL_APP_CONTINUE; /* carry on with the program! */
}





/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
	game.handleInput(input);

	static Uint64 lastTicks = 0;
	auto currentTicks = SDL_GetTicks();
	auto elapsedMillisecs = currentTicks - lastTicks; // how many milliseconds elapsed since last frame?
	lastTicks = currentTicks;
	auto elapsedSecs = elapsedMillisecs / 1000.0f;
	auto lagSecs = elapsedSecs;

	while (lagSecs > 0) {		
		game.update(engine.get(), gameUpdateStepSecs);
		lagSecs -= gameUpdateStepSecs;
		lagSecs = SDL_max(0, lagSecs);
	}

	SDL_SetRenderDrawColor(renderer, 230, 230, 230, SDL_ALPHA_OPAQUE);

	SDL_RenderClear(renderer);

	game.draw(engine.get());

	SDL_RenderPresent(renderer);

	input = {};

	return SDL_APP_CONTINUE; /* carry on with the program! */
}




/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	/* SDL will clean up the window/renderer for us. */
}
