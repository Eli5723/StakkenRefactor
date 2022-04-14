#pragma once

#include <array>
#include <vector>
#include <map>
#include <filesystem>

#include <Assets/Sound.h>

struct Sounds {
    enum class Slot {
        Clear,
        SonicDrop,
        HardDrop,
        Lock,
        Combo5,
        Combo7,
        Combo10,
        Combo12,
        Combo14,
        Win,
        Reject,
        Click,
        Count,
    };

    static int enabled;
    static std::array<Assets::Sound*, (size_t)Slot::Count> sounds;

    static void loadDirectory(const std::filesystem::path& path);
    static bool valid(const std::filesystem::path& file);

    static void play(Slot slot);
    static void Toggle();
};
