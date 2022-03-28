#include <Assets/Sounds.h>

#include <SDL2/SDL.h>

using ExtensionList = std::vector<std::string>;
const ExtensionList soundExtensions = { ".wav",".mp3",".ogg" };

std::map<std::string, int> soundNames{
    {"Clear"    , (size_t)Sounds::Slot::Clear},
    {"SonicDrop", (size_t)Sounds::Slot::SonicDrop},
    {"HardDrop" , (size_t)Sounds::Slot::HardDrop},
    {"Lock"     , (size_t)Sounds::Slot::Lock},
    {"Combo5"   , (size_t)Sounds::Slot::Combo5},
    {"Combo7"   , (size_t)Sounds::Slot::Combo7},
    {"Combo10"  , (size_t)Sounds::Slot::Combo10},
    {"Combo12"  , (size_t)Sounds::Slot::Combo12},
    {"Combo14"  , (size_t)Sounds::Slot::Combo14},
    {"Win"      , (size_t)Sounds::Slot::Win},
    {"Reject"   , (size_t)Sounds::Slot::Reject},
    {"Click"   , (size_t)Sounds::Slot::Click}
};

int Sounds::enabled = true;
std::array<Assets::Sound*, (size_t)Sounds::Slot::Count> Sounds::sounds;

void Sounds::loadDirectory(const std::filesystem::path& path) {
    for (auto& p : std::filesystem::directory_iterator(path)) {
        std::filesystem::path file = p.path();
        std::string name = p.path().stem().string();

        if (!Sounds::valid(file))
            continue;

        auto it = soundNames.find(name);
        if (it != soundNames.end()) {
            int index = it->second;
            if (sounds[index]) {
                SDL_Log("Loaded duplicate sound: %s", name.c_str());
                delete sounds[index];
            }

            Assets::Sound* s = Assets::Sound::Load(file);

            if (s)
                sounds[index] = s;
            else
                SDL_Log("Failed to load sound: %s", name.c_str());
        }
    }
}

void Sounds::play(Sounds::Slot slot) {
    if (!Sounds::enabled)
        return;

    if (sounds[(size_t)slot] != nullptr)
        sounds[(size_t)slot]->play();
}

bool Sounds::valid(const std::filesystem::path& file) {
    std::filesystem::path fileExtension = file.extension();

    if (std::find(soundExtensions.begin(), soundExtensions.end(), fileExtension) != soundExtensions.end())
        return true;

    return false;
}

void Sounds::Toggle() {
    // if (enabled) {
    //     UI::AddToScreen(new UI::Notification("Audio Muted.",2000));
    // } else {
    //     UI::AddToScreen(new UI::Notification("Audio Resumed.",2000));
    // }

    enabled = !enabled;
}
