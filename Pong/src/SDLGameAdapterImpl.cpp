module SDLGameAdapter;
using namespace  std;
import Pong;
import SDLEngine;

unique_ptr<Pong> pong{ nullptr };


void  SDLGameAdapter::init(SDL_Renderer* renderer, const string& image_folder, const string& sound_folder)
{
	pong = make_unique<Pong>(make_unique<SDLEngine>(
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


void  SDLGameAdapter::handleInput(SDL_Event*)
{
	pong->handleInput(Input());
}




void  SDLGameAdapter::update(float elapsedSecs)
{
	pong->update(elapsedSecs);
}

void SDLGameAdapter::quit()
{
	pong.reset();
}




