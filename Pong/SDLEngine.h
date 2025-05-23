#pragma once
#include "Engine.h"
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include <memory>


class SDLEngine :public Engine {

public:

	SDLEngine(SDL_Renderer* renderer, const std::string& images_subfolder);


	void draw(const std::string& image, float x, float y) override;

	void play(const std::string& sound) override;

	~SDLEngine();

private:

	SDL_Renderer* renderer = nullptr;
	const std::string images_subfolder;
	std::unordered_map<std::string, SDL_Texture*> textures;
	
};


static bool load_image(SDL_Renderer* renderer, const char* dirname, const char* fname, SDL_Texture** result);
