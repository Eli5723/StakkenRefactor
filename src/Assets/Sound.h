#pragma once

#include <filesystem>
#include <string>

#include <SDL2/SDL_mixer.h>

namespace Assets {

struct Sound {
    std::string name;
    Mix_Chunk* chunk = nullptr;

    static void Init();
    static Sound* Load(const std::filesystem::path& path);

    void play();
};

}