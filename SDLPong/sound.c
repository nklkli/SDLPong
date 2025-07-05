#include "sound.h"

#include <assert.h>
#include <SDL3/SDL_mixer.h>

static int NUM_CHANNELS = 8;
static SDL_PropertiesID PROP_SOUNDS;



static void  CleanupPropertyCallback(void* userdata, void* value)
{
	Mix_FreeChunk(value);
	SDL_Log("Sound freed: %s", (char*)userdata);
	SDL_free(userdata);
}


bool soundInit(void)
{

	if (!Mix_OpenAudio(0, NULL)) {
		SDL_Log("Mix_OpenAudio failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		return false;
	}

	int numChannels = Mix_AllocateChannels(NUM_CHANNELS);
	SDL_Log("Num audio channels: %d", numChannels);

	if ((PROP_SOUNDS = SDL_CreateProperties()) == 0) {
		SDL_Log("SDL_CreateProperties failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		return false;
	}

	char* sound_folder;
	{
		int result = SDL_asprintf(&sound_folder, "%sassets/sounds", SDL_GetBasePath());

		if (result < 0) {
			SDL_Log("SDL_asprintf failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}
	}

	int count = 0;
	char** files = SDL_GlobDirectory(sound_folder, "*.*", 0, &count);
	SDL_free(sound_folder);
	if (!files) {
		SDL_Log("SDL_GlobDirectory failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		char* soundPath;
		{
			int result = SDL_asprintf(&soundPath, "%sassets/sounds/%s", SDL_GetBasePath(), files[i]);
			if (result < 0) {
				SDL_Log("SDL_snprintf failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
				return false;
			}
		}

		Mix_Chunk* chunk = Mix_LoadWAV(soundPath);

		if (!chunk) {
			SDL_Log("Mix_LoadWAV failed in file %s[%d]\t\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}

		bool result = SDL_SetPointerPropertyWithCleanup(
			PROP_SOUNDS,
			files[i],
			chunk,
			CleanupPropertyCallback,
			soundPath);


		if (!result) {
			SDL_Log("SDL_SetPointerPropertyWithCleanup failed in file %s[%d]\n%s", __FILE__, __LINE__, SDL_GetError());
			return false;
		}

		SDL_Log("Sound loaded: %s", (char*)soundPath);		
	}


	return true;
}



void soundPlay(const char* sound)
{
	Mix_Chunk * chunk = SDL_GetPointerProperty(PROP_SOUNDS, sound, NULL);
	if (!chunk) {
		SDL_Log("Sound '%s' not found", sound);
	}
	else {
		/*int channel_playing =*/ Mix_PlayChannel(-1, chunk, 0);

		//SDL_Log("Playing '%s' on channel %d.", sound, channel_playing);
	}
}


void soundQuit(void)
{
	Mix_CloseAudio();
	SDL_DestroyProperties(PROP_SOUNDS);
}