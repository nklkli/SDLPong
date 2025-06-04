module SDLGameAdapter;
using namespace  std;
import Pong;
import SDLEngine;

SDLGameAdapter::SDLGameAdapter()
{
}

int SDLGameAdapter::Get_WINDOWS_HEIGHT()
{
	return 0;
}

int SDLGameAdapter::Get_WINDOWS_WIDTH()
{
	return 0;
}

void SDLGameAdapter::draw()
{
	
}


void SDLGameAdapter::handleInput(SDL_Event*)
{
	
}

void SDLGameAdapter::init(SDL_Renderer* renderer, const string& image_folder, const string& sound_folder)
{
	game_ = new Pong(make_unique<EngineSDL>(renderer, image_folder, sound_folder));
}

void SDLGameAdapter::update(float)
{
	
}




