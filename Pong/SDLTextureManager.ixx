module;
#include <SDL_image.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
export module SDLTextureManager;
import std;
using namespace std;

export
class SDLTextureManager
{
	SDL_Renderer* renderer_ = nullptr;
	unordered_map<string, SDL_Texture*> textures_;
public:
	SDLTextureManager(SDL_Renderer* renderer, const string& image_folder);
	void render(const string& image, float x, float y) const;
	~SDLTextureManager();
};

module : private;

import  Util;

SDL_Texture* create_texture_from_file(SDL_Renderer* renderer, const char* path);


SDLTextureManager::SDLTextureManager(SDL_Renderer* renderer, const string& image_folder):
	renderer_(renderer)
{
	
	// iterate over files in 'image_folder' and create an SDL_Texture for each file
	for (const auto& entry : filesystem::directory_iterator(image_folder)) {
		if (!entry.is_regular_file())
		{
			// ignore everything in 'image_folder' except normal files
			continue;
		}

	/*	auto path = entry.path().string().c_str();
		auto path2 = entry.path().generic_u8string().c_str();
		auto path3 = entry.path().string();
		auto path4 = entry.path().u8string().c_str();
		auto path5 = entry.path().generic_string().c_str();
		auto path6 = entry.path().generic_wstring();*/

		auto path = Util::utf16ToUtf8(entry.path().generic_wstring());
		auto filename_no_extension = entry.path().stem().string();
		textures_[filename_no_extension] = create_texture_from_file(renderer, path.c_str());
		SDL_Log("SDLTextureManager: texture  loaded '%s'", path.c_str());
	}
}


void SDLTextureManager::render(const string& image, float x, float y) const
{
	auto texture = textures_.at(image);
	SDL_FRect rect = 
	{
		x,
		y,
		static_cast<float>(texture->w),
		static_cast<float>(texture->h)
	};
	SDL_RenderTexture(renderer_, texture, nullptr, &rect);
}




SDLTextureManager::~SDLTextureManager()
{
	println("SDLTextureManager dtor");
	for (const auto& p : textures_) {
		SDL_DestroyTexture(p.second);
		SDL_Log("\tTexture '%s' destroyed.", p.first.c_str());
	}
}


SDL_Texture* create_texture_from_file(SDL_Renderer* renderer, const char* path)
{

	//SDL_IOStream * s = SDL_IOFromMem(NULL, 0);

	//SDL_Surface* surface = IMG_Load_IO(s, true);

	SDL_Surface* surface = IMG_Load(path);

	if (!surface) {
		throw std::format("SDLTextureManager: Couldn't create SDL_Surface from file '{}'. Reason: \n{}",
			path,
			SDL_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
	if (!texture) {
		throw format("SDLTextureManager: Couldn't create SDL texture from '{}'. Reason:\n{}", path, SDL_GetError());
	}
	return texture;
}
