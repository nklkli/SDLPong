#include "SDLEngine.h"
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL_image.h>
#include <format>
#include <print>

SDLEngine::SDLEngine(SDL_Renderer* _renderer, const std::string& _images_subfolder) :
	renderer{ _renderer },
	images_subfolder{_images_subfolder}
{

	auto callback = [](void* userdata, const char* dirname, const char* fname)
		{
			//return SDL_ENUM_FAILURE;
			//SDL_Log("Load %s%s", dirname, fname);
			std::print("Load {}{}\n", dirname, fname);
			
			return SDL_ENUM_CONTINUE;
		};

	auto images_folder = std::format("{}{}", SDL_GetBasePath(), _images_subfolder);

	SDL_EnumerateDirectory(images_folder.c_str(), callback, NULL);
}

void SDLEngine::draw(const std::string& image, float x, float y)
{

}

void SDLEngine::play(const std::string& sound)
{

}



bool load_image(SDL_Renderer* renderer, const char* dirname, const char* fname, SDL_Texture** result) {
	char* path = NULL;
	SDL_asprintf(&path, "%s%s/%s", SDL_GetBasePath(), dirname, fname);
	if (!SDL_GetPathInfo(path, NULL)) { //  image file not found
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "%s   %s", SDL_GetError(), path);
		return false;
	}
	SDL_Surface* surface = IMG_Load(path);
	SDL_free(path);
	if (!surface) {
		SDL_Log("Couldn't load image: %s", SDL_GetError());
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
	if (!texture) {
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "IMAGE: Couldn't create texture: %s", SDL_GetError());
		return false;
	}
	*result = texture;
	return true;
}