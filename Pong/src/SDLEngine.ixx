module;
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>
export module SDLEngine;
import std;
import Point;
import SDLTextureManager;
import SDLSoundManager;
export import Engine;
using namespace std;

export
class EngineSDL :public Engine {
	unique_ptr<SDLTextureManager> textureManager_{ nullptr };
	unique_ptr<SDLSoundManager> soundManager_{ nullptr };
	SDL_Renderer* renderer_ = nullptr;
public:
	EngineSDL(SDL_Renderer* _renderer, const string& _images_subfolder, const string& sound_folder);
	void draw(const string& image, const Point& pos) const override;
	void drawText(const string& text, const Point& pos) const override;
	void play(const string& sound) const override;
	~EngineSDL() override;
};



module : private;



EngineSDL::EngineSDL(SDL_Renderer* renderer,
	const string& images_folder,
	const string& sound_folder) :
	renderer_(renderer)
{
	textureManager_.reset(new SDLTextureManager(renderer, images_folder));
	soundManager_.reset(new SDLSoundManager(sound_folder));
}

void EngineSDL::draw(const string& image, const Point& pos) const
{
	textureManager_->render(image, pos.x, pos.y);
}

void EngineSDL::drawText(const string& text, const Point& pos) const
{
	SDL_RenderDebugText(renderer_, pos.x, pos.y, text.c_str());
}

void EngineSDL::play(const string& sound) const {
	soundManager_->play(sound);
}

EngineSDL::~EngineSDL()
{
	SDL_Log("SDLEngine dtor:");
}


