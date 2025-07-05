#include <assert.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include "draw.h"


static SDL_PropertiesID PROP_TEXTURES;
static void cleanupTextureCallback(void*, void*);


bool drawInit(SDL_Renderer* renderer)
{
	if ((PROP_TEXTURES = SDL_CreateProperties()) == 0) {
		SDL_Log("SDL_CreateProperties failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		return false;
	}

	char* image_folder;
	{
		int result = SDL_asprintf(&image_folder, "%sassets/images", SDL_GetBasePath());

		if (result < 0) {
			SDL_Log("SDL_asprintf failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}
	}

	int count = 0;
	char** files = SDL_GlobDirectory(image_folder, "*.png", 0, &count);
	SDL_free(image_folder);
	if (!files) {
		SDL_Log("SDL_GlobDirectory failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		char* texturePath;
		{
			int result = SDL_asprintf(&texturePath, "%sassets/images/%s", SDL_GetBasePath(), files[i]);
			if (result < 0) {
				SDL_Log("SDL_snprintf failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
				return false;
			}
		}

		SDL_Surface* surface = IMG_Load(texturePath);

		if (!surface) {
			SDL_Log("IMG_Load failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
		if (!texture) {
			SDL_Log("SDL_CreateTextureFromSurface failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}


		bool result = SDL_SetPointerPropertyWithCleanup(
			PROP_TEXTURES,
			files[i],
			texture,
			cleanupTextureCallback,
			texturePath);

		if (!result) 
		{
			SDL_Log("SDL_SetPointerPropertyWithCleanup failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}

		SDL_Log("Texture loaded: %s", (char*)texturePath);
	}

	return true;
}


void drawQuit(void)
{
	SDL_DestroyProperties(PROP_TEXTURES);
}


void drawImage(SDL_Renderer* renderer, const char* image, float x, float y, bool center)
{
	SDL_Texture* texture = SDL_GetPointerProperty(PROP_TEXTURES, image, NULL);

	if (!texture)
	{
		SDL_Log("Texture '%s' not found", image);
	}
	else {



		//SDL_RenderTextureRotated()

		if (center) {
			x = x - (float)texture->w / 2.0f;
			y = y - (float)texture->h / 2.0f;
		}

		SDL_RenderTexture(
			renderer,
			texture,
			NULL,
			&(SDL_FRect){x, y, (float)texture->w, (float)texture->h});
	}
}








void cleanupTextureCallback(void* userdata, void* value)
{
	assert(value != NULL);
	SDL_DestroyTexture(value);
	SDL_Log("Texture cleaned up: %s", (char*)userdata);
	SDL_free(userdata);
}

