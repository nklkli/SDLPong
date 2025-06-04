module;
#include <SDL_image.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <typeinfo>
export module SDLTextureManager;
import std;
using namespace std;


class Image
{
	SDL_Texture* texture_{ nullptr };
	string path_;
public:
	Image(const string& path, SDL_Renderer* renderer);
	SDL_Texture* GetTexture() const;
	~Image();
};

export
class SDLTextureManager
{
	SDL_Renderer* renderer_ = nullptr;
	unordered_map<string, unique_ptr<Image>> images_;
public:
	SDLTextureManager(SDL_Renderer* renderer, const string& image_folder);
	void render(const string& image, float x, float y) const;
	~SDLTextureManager();
};


module : private;


import  Util;


Image::Image(const string& path, SDL_Renderer* renderer) :path_{ path }
{
	//SDL_IOStream * s = SDL_IOFromMem(NULL, 0);
	//SDL_Surface* surface = IMG_Load_IO(s, true);
	SDL_Surface* surface = IMG_Load(path.c_str());
	
	if (!surface) {
		throw std::format("{}: Couldn't create SDL_Surface from file '{}'. Reason: \n{}",
			typeid(Image).name(),
			path,
			SDL_GetError());
	}

	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
	if (!texture_) {
		throw format("{}:  Couldn't create SDL texture from '{}'. Reason:\n{}",
			typeid(Image).name(),
			path,
			SDL_GetError());
	}
	SDL_Log("%s: SDLTextureManager: texture  loaded '%s'",
		typeid(Image).name(),
		path.c_str());
}

SDL_Texture* Image::GetTexture() const
{
	return texture_;
}


Image::~Image()
{
	SDL_DestroyTexture(texture_);
	SDL_Log("%s: Texture destroyed: '%s'",
		typeid(Image).name(),
		path_.c_str());
}

SDLTextureManager::SDLTextureManager(SDL_Renderer* renderer, const string& image_folder) :
	renderer_(renderer)
{

	// iterate over files in 'image_folder' and create an SDL_Texture for each file
	for (const auto& entry : filesystem::directory_iterator(image_folder)) {
		if (!entry.is_regular_file())
		{
			// ignore everything in 'image_folder' except normal files
			continue;
		}

		auto filename_no_extension = entry.path().stem().string();

		images_.emplace(
			filename_no_extension,
			make_unique<Image>(
				entry.path().string(),
				renderer
			)
		);
	}
}


void SDLTextureManager::render(const string& image_name, float x, float y) const
{
	auto image = images_.at(image_name).get();
	SDL_Texture* texture = image->GetTexture();
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
	println("{} dtor", typeid(SDLTextureManager).name());
}


