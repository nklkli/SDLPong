module;
#include <SDL3/SDL_render.h>
export module SDLGameAdapter;
export import std;

export
namespace SDLGameAdapter
{
	void init(SDL_Renderer* renderer, const std::string& image_folder, const std::string& sound_folder);
	void quit();
	int Get_WINDOWS_WIDTH();
	int Get_WINDOWS_HEIGHT();
	void update(float);
	void handleInput(SDL_Event*);
	void draw();
}








