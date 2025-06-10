module;
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>
export module SDLEngine;
import std;
export import  Point;
import SDLTextureManager;
import SDLSoundManager;
export import Engine;
using std::string, std::unique_ptr;

export
class SDLEngine :public Engine {
	unique_ptr<SDLTextureManager> textureManager_{ nullptr };
	unique_ptr<SDLSoundManager> soundManager_{ nullptr };
	SDL_Renderer* renderer_ = nullptr;
public:
	SDLEngine(SDL_Renderer* _renderer, const string& images_subfolder, const string& sound_folder);
	void draw(const string& image, const Point& pos) const override;
	void drawText(const string& text, const Point& pos) const override;
	void play(const string& sound) const override;
	~SDLEngine() override;
	
};



module : private;



SDLEngine::SDLEngine(SDL_Renderer* renderer,
	const string& images_folder,
	const string& sound_folder) :
	renderer_(renderer)
{
	textureManager_.reset(new SDLTextureManager(renderer, images_folder));
	soundManager_.reset(new SDLSoundManager(sound_folder));
}

void SDLEngine::draw(const string& image, const Point& pos) const
{
	textureManager_->render(image, pos.x, pos.y);
}

void SDLEngine::drawText(const string& text, const Point& pos) const
{
	SDL_RenderDebugText(renderer_, pos.x, pos.y, text.c_str());
}

void SDLEngine::play(const string& sound) const {
	soundManager_->play(sound);
}

SDLEngine::~SDLEngine()
{
	SDL_Log("SDLEngine dtor:");
}



