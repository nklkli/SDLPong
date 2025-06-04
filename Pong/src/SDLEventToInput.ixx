module;
#include <SDL3/SDL_events.h>
export module SDLEventToInput;
import Input;

export
Input mapSDLEventToInput(const SDL_Event* event)
{
	Input input;
	switch (event->type)
	{
	case SDL_EVENT_KEY_DOWN:
		if (event->key.scancode == SDL_SCANCODE_UP)
		{
			input.ArrowUpPressed = true;
		}
		else if(event->key.scancode == SDL_SCANCODE_DOWN)
		{
			input.ArrawDownPressed = true;
		}
		else if (event->key.scancode==SDL_SCANCODE_SPACE)
		{
			input.SpacebarPressed = true;
		}
		break;
	default:
		break;

	}

	return input;
}
