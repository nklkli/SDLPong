module;
#include <SDL_mixer.h>
#include <typeinfo>
#include <SDL3/SDL_log.h>
export module SDLSoundManager;
import std;
using namespace std;


/// <summary>
/// Loads and destroys SDL_mixer's Mix_Chunk.
/// </summary>
class Sound
{
	const string name_;
	Mix_Chunk* chunk_{ nullptr };
public:
	Sound(const string& path);
	operator Mix_Chunk* () const;
	const string& GetName() const;
	~Sound();
};

export
class SDLSoundManager
{
	unordered_map<string, unique_ptr<Sound>>  sounds_;
	int num_channels_{ 16 };

public:
	SDLSoundManager(const string& sound_folder);
	void play(const string& sound) const;
	~SDLSoundManager();
};



module :private;




Sound::Sound(const string& path) :
	name_{ path }
{
	chunk_ = Mix_LoadWAV(path.c_str());
	if (!chunk_) {
		throw  format("{}: Couldn't load sound file '{}';\n{}",
			typeid(*this).name(),
			path,
			SDL_GetError());
	}

	chunk_->volume = 20;

	SDL_Log("%s: loaded '%s'",
		typeid(*this).name(),
		path.c_str());
}

Sound::operator Mix_Chunk*() const
{ return chunk_; }

const string& Sound::GetName() const
{ return name_; }

Sound::~Sound()
{
	Mix_FreeChunk(chunk_);
	SDL_Log("%s: Sound destroyed: %s",
		typeid(*this).name(),
		name_.c_str());
}

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

		auto filename_no_extension = entry.path().stem().string();
		auto path = entry.path().string();

		sounds_.emplace(
			filename_no_extension,
			make_unique<Sound>(path)
		);


	}
}



void SDLSoundManager::play(const string& sound_name) const
{
	auto& sound = *sounds_.at(sound_name).get();
	Mix_PlayChannel(-1, sound, 0);
	SDL_Log("%s: Playing sound file '%s'",
		typeid(*this).name(),
		sound.GetName().c_str());
}


SDLSoundManager::~SDLSoundManager()
{
	SDL_Log("SDLSoundManager dtor");
	sounds_.clear();
	Mix_CloseAudio();
	Mix_Quit();
}




