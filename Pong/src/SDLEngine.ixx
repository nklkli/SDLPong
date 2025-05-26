module;

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL_image.h>

export module SDLEngine;

import Engine;


export
class SDLEngine :public Engine {

	SDL_Renderer* renderer = nullptr;
	const std::string images_subfolder;
	std::unordered_map<std::string, SDL_Texture*> textures;


public:

	SDLEngine(SDL_Renderer* renderer, const std::string& images_subfolder);

	void draw(const Image&, const Point&) const override;

	void play(const std::string& sound) override;

	~SDLEngine() override;



};

module : private;

static bool load_image(SDL_Renderer* renderer, const char* dirname, const char* fname, SDL_Texture** result);


std::string get_filename_without_extension(const std::string fname_with_extension) {
	auto dot_pos = fname_with_extension.find_last_of(".");
	return fname_with_extension.substr(0, dot_pos);
}


SDLEngine::SDLEngine(SDL_Renderer* _renderer, const std::string& _images_subfolder) :
	renderer{ _renderer },
	images_subfolder{ _images_subfolder }
{
	auto callback = [](void* userdata, const char* dirname, const char* fname)
		{
			SDLEngine* self = static_cast<SDLEngine*>(userdata);
			SDL_Texture* texture_ptr;
			if (load_image(self->renderer, dirname, fname, &texture_ptr)) {
				self->textures[get_filename_without_extension(fname)] = texture_ptr;
				SDL_Log("SDLEngine: texture  loaded '%s' '%s'", dirname, fname);
			}
			return SDL_ENUM_CONTINUE;
		};

	auto images_folder = std::format("{}{}", SDL_GetBasePath(), _images_subfolder);
	SDL_EnumerateDirectory(images_folder.c_str(), callback, this);
}

void SDLEngine::draw(const Image& image, const Point& pos) const
{
	auto texture = textures.at(image.name);
	SDL_FRect rect = { pos.x,pos.y, (float)texture->w, (float)texture->h };
	SDL_RenderTexture(renderer, texture, NULL, &rect);
}




void SDLEngine::play(const std::string& sound)
{

}


SDLEngine::~SDLEngine()
{
	SDL_Log("SDLEngine dtor:");
	for (const auto& p : textures) {
		SDL_DestroyTexture(p.second);
		SDL_Log("\tTexture '%s' destroyed.", p.first.c_str());
	}
}



bool load_image(SDL_Renderer* renderer, const char* dirname, const char* fname, SDL_Texture** result) {
	char* path = NULL;
	SDL_asprintf(&path, "%s/%s", dirname, fname);
	SDL_PathInfo pathInfo;
	if (!SDL_GetPathInfo(path, &pathInfo)) { //  image file not found
		throw std::format("SDLEngine: error accessing file '{}' in folder '{}'. Reason:\n{}",
			fname, dirname, SDL_GetError());
	}

	if (pathInfo.type != SDL_PATHTYPE_FILE) {
		return false;
	}

	//SDL_IOStream * s = SDL_IOFromMem(NULL, 0);

	//SDL_Surface* surface = IMG_Load_IO(s, true);

	SDL_Surface* surface = IMG_Load(path);
	SDL_free(path);
	if (!surface) {
		throw std::format("SDLEngine: Couldn't create SDL_Surface from file '{}'. Reason: \n{}", path, SDL_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
	if (!texture) {
		throw std::format("SDLEngine: Couldn't create SDL texture from '{}'. Reason:\n{}", fname, SDL_GetError());
	}
	*result = texture;

	return true;
}

