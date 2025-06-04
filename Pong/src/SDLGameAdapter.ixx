module;
#include <SDL3/SDL_render.h>
export module SDLGameAdapter;
import Game;
export import std;
using namespace  std;


export class SDLGameAdapter
{
	Game* game_{nullptr};
public:
	SDLGameAdapter();
	int Get_WINDOWS_WIDTH() ;
	int Get_WINDOWS_HEIGHT() ;
	void update(float) ;
	void handleInput(SDL_Event*) ;
	void init(SDL_Renderer* renderer, const string& image_folder, const string& sound_folder) ;
	void draw() ;
};






