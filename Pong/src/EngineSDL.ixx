module;
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL_image.h>
#include <SDL3/SDL_events.h>
export module EngineSDL;
import std;
import Engine;
using namespace std;


export
class EngineSDL :public Engine {
	SDL_Renderer* renderer = nullptr;
	const string images_subfolder;
	unordered_map<string, SDL_Texture*> textures;
public:
	EngineSDL(SDL_Renderer* _renderer, const string& _images_subfolder);
	void draw(const string& image, const Point& pos) const override;
	void drawText(const string& text, const Point& pos) const override;
	void play(const string& sound) const;
	~EngineSDL() override;
};




module : private;


string get_filename_without_extension(const string& fname_with_extension)
{
	auto dot_pos = fname_with_extension.find_last_of(".");
	return fname_with_extension.substr(0, dot_pos);
}


bool load_image(SDL_Renderer* renderer, const char* dirname, const char* fname, SDL_Texture** result) {
	char* path = NULL;
	SDL_asprintf(&path, "%s/%s", dirname, fname);
	SDL_PathInfo pathInfo;
	if (!SDL_GetPathInfo(path, &pathInfo)) { //  image file not found
		throw format("SDLEngine: error accessing file '{}' in folder '{}'. Reason:\n{}",
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
		throw format("SDLEngine: Couldn't create SDL_Surface from file '{}'. Reason: \n{}", path, SDL_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
	if (!texture) {
		throw format("SDLEngine: Couldn't create SDL texture from '{}'. Reason:\n{}", fname, SDL_GetError());
	}
	*result = texture;

	return true;
}

EngineSDL::EngineSDL(SDL_Renderer* _renderer, const string& _images_subfolder) :
	renderer{ _renderer },
	images_subfolder{ _images_subfolder }
{
	auto callback = [](void* userdata, const char* dirname, const char* fname)
		{
			EngineSDL* self = static_cast<EngineSDL*>(userdata);
			SDL_Texture* texture_ptr;
			if (load_image(self->renderer, dirname, fname, &texture_ptr)) {
				self->textures[get_filename_without_extension(fname)] = texture_ptr;
				SDL_Log("SDLEngine: texture  loaded '%s' '%s'", dirname, fname);
			}
			return SDL_ENUM_CONTINUE;
		};

	auto images_folder = format("{}{}", SDL_GetBasePath(), _images_subfolder);
	SDL_EnumerateDirectory(images_folder.c_str(), callback, this);
}

void EngineSDL::draw(const string& image, const Point& pos) const
{
	auto texture = textures.at(image);
	SDL_FRect rect = { pos.x, pos.y, (float)texture->w, (float)texture->h };
	SDL_RenderTexture(renderer, texture, NULL, &rect);
}

void EngineSDL::drawText(const string& text, const Point& pos) const
{
	SDL_RenderDebugText(renderer, pos.x, pos.y, text.c_str());
}

void EngineSDL::play(const string& sound) const {
}

EngineSDL::~EngineSDL()
{
	SDL_Log("SDLEngine dtor:");
	for (const auto& p : textures) {
		SDL_DestroyTexture(p.second);
		SDL_Log("\tTexture '%s' destroyed.", p.first.c_str());
	}
}

