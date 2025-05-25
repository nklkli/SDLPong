#pragma once
#include "Engine.h"
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include <memory>
#include "Image.h"

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

static bool load_image(SDL_Renderer *renderer, const char *dirname, const char *fname, SDL_Texture **result);
