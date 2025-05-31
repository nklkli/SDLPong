module;
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
export module SDLEventToCommand;
import Command;

export 
void handleInput(const SDL_Event * event, Command& command)
{
	switch (event->type)
	{
	case SDL_EVENT_KEY_DOWN:
		if (event->key.scancode == SDL_SCANCODE_RIGHT)
		{
			command.NextState = true;
		}
		break;
	default:
		break;

	}
}
