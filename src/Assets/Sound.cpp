#include <Assets/Sound.h>

#include <SDL2/SDL.h>

namespace Assets {

    void Sound::Init(){
        if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,4,2048) < 0){
            SDL_Log("Failed to Load audio.");
        }
    }

    // Load new fragment shader
    Sound* Sound::Load(const std::filesystem::path& path){
        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        if (!chunk){
            SDL_Log("Failed to Load sound at %s", path.c_str());
            return nullptr;
        }

        Sound* loaded = new Sound;
        loaded->chunk = chunk;
        loaded->name = path.stem();
        
        return loaded;
    }

    void Sound::play(){
        Mix_PlayChannel(-1,chunk,0);
    }
}
