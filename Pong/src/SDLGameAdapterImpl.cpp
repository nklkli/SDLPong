module;
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
module SDLGameAdapter;
import Pong;
import SDLEngine;
using std::string,std::unique_ptr;

unique_ptr<Pong> pong{ nullptr };


void  SDLGameAdapter::init(SDL_Renderer* renderer, const string& image_folder, const string& sound_folder)
{
	pong = make_unique<Pong>(
		make_unique<SDLEngine>(
			renderer,
			image_folder,
			sound_folder));
}



int  SDLGameAdapter::Get_WINDOWS_HEIGHT()
{
	return static_cast<int>(Pong::HEIGHT);
}


int  SDLGameAdapter::Get_WINDOWS_WIDTH()
{
	return static_cast<int>(Pong::WIDTH);
}


void  SDLGameAdapter::draw()
{
	pong->draw();
}


void  SDLGameAdapter::handleInput(SDL_Event* event)
{
	pong->handleInput(event);
}




void  SDLGameAdapter::update()
{
	static Uint64 last_ticks = 0;
	const auto current_ticks = SDL_GetTicks();
	const auto elapsed_milliseconds = current_ticks - last_ticks; // how many milliseconds elapsed since last frame?
	last_ticks = current_ticks;
	const auto elapsed_secs = elapsed_milliseconds / 1000.0f;
	auto lag_secs = elapsed_secs;

	while (lag_secs > 0) {
		auto update_step = SDL_min(lag_secs, Pong::gameUpdateStepSecs);
		pong->update(update_step);
		lag_secs -= Pong::gameUpdateStepSecs;
	}
}

void SDLGameAdapter::quit()
{
	pong.reset();
}




