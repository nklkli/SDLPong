module;
#include <SDL_mixer.h>
#include <SDL3/SDL_log.h>
export module SDLSoundManager;
import std;
using namespace std;

 void DestroyChunk(Mix_Chunk* chunk)
{
	Mix_FreeChunk(chunk);
	SDL_Log("Destroy sound");
}

export
class SDLSoundManager
{
	unordered_map<string, unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)>>  mix_chunks_;
	int num_channels_{ 16 };

public:
	SDLSoundManager(const string& sound_folder);
	void play(const string& sound) const;
	~SDLSoundManager();
};



module :private;
import Util;



Mix_Chunk* load_sound(const char* path);

SDLSoundManager::SDLSoundManager(const string& sound_folder)
{
	SDL_Log("SDLSoundManager: SDL_MIXER VERSION %d.%d.%d", 
		SDL_MIXER_MAJOR_VERSION,
		SDL_MINOR_VERSION,
		SDL_MIXER_MICRO_VERSION);

	if (!Mix_OpenAudio(0, nullptr)) {
		throw format("Error sound init: %s", SDL_GetError());
	}
	int numChannels = Mix_AllocateChannels(num_channels_);
	SDL_Log("Audio channels: %d", numChannels);

	for (const auto& entry : filesystem::directory_iterator(sound_folder)) {
		if (!entry.is_regular_file())
		{
			// ignore everything in 'sound_folder' except normal files.
			continue;
		}
		auto path = Util::utf16ToUtf8(entry.path().generic_wstring());
		auto filename_no_extension = entry.path().stem().string();

		unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)> p{load_sound(path.c_str()), DestroyChunk};
		
		mix_chunks_.emplace(
			filename_no_extension,
			move(p)
		);

		SDL_Log("\tSound  loaded '%s'", path.c_str());
	}
}



void SDLSoundManager::play(const string& sound) const
{
	auto mix_chunk = mix_chunks_.at(sound).get();
	Mix_PlayChannel(-1, mix_chunk, 0);

	//int channel_playing = Mix_PlayChannel(-1, mix_chunk, 0);
		/*	SDL_LogInfo(LOG_CATEGORY, "Playing '%s' on channel %d.", soundFile->path, channel_playing);
		}
		else {
			SDL_LogWarn(LOG_CATEGORY, "Sound '%s' not loaded.", name);
		}*/
}


SDLSoundManager::~SDLSoundManager()
{
	SDL_Log("SDLSoundManager dtor");

	
	/*for (auto& [sound_name, chunk]:mix_chunks_)
	{
		Mix_FreeChunk(chunk);
		SDL_Log("\tSound destroyed '%s'", sound_name.c_str());
	}*/
	/*for (const auto& p : mix_chunks_)
	{
		Mix_FreeChunk(p.second);
		SDL_Log("\tSound destroyed '%s'", p.first.c_str());
	}*/
//	Mix_CloseAudio();
	//Mix_Quit();
}


Mix_Chunk* load_sound(const char* path) {
	Mix_Chunk* mix_chunk = Mix_LoadWAV(path);
	if (!mix_chunk) {
		throw  format("Couldn't load sound file '{}';\n%s",
			path, SDL_GetError());
	}
	return mix_chunk;
}
