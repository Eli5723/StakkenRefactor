#include <Input/InputProfile.h>

#include <SDL2/SDL.h>

namespace Input {

    InputProfile::InputProfile() {
        name = "Default";

        das = 110;
        arr = 0;
        dropArr = 100;

        // Piece Movement settings
        left = SDL_SCANCODE_A;
        right = SDL_SCANCODE_D;

        softDrop = SDL_SCANCODE_KP_5;
        hardDrop = SDL_SCANCODE_SPACE;
        sonicDrop = SDL_SCANCODE_LSHIFT;

        rcw = SDL_SCANCODE_KP_6;
        rccw = SDL_SCANCODE_KP_4;
        flip = SDL_SCANCODE_KP_8;

        for (int i = 0; i < 7; i++)
            rotation[i] = 0;
    }

// Load existing profile from file
    InputProfile *InputProfile::Load(const std::string &name) {
        InputProfile *loaded = new InputProfile;
        loaded->name = name;

        std::filesystem::path path = PROFILE_DIRECTORY;
        path += name;
        path += FILE_EXTENSION;
    
        std::ifstream input(path);

        if (!input) {
            SDL_Log("Failed to Load keyboard profile \"%s\".", name.c_str());
            return 0;
        }

        input >> loaded->das;
        input >> loaded->arr;
        input >> loaded->dropArr;

        input >> loaded->T_rotation;
        input >> loaded->O_rotation;
        input >> loaded->L_rotation;
        input >> loaded->J_rotation;
        input >> loaded->Z_rotation;
        input >> loaded->S_rotation;
        input >> loaded->I_rotation;

        input.ignore();

        std::string keyname;
        for (int i = 0; i < 8; i++) {
            std::getline(input, keyname);
            
            SDL_Scancode key = SDL_GetScancodeFromName(keyname.c_str()); 
            loaded->keys[i] = key;
            
            if (key == SDL_SCANCODE_UNKNOWN){
                SDL_Log("Unknown keyname in profile %s.", name.c_str());
            }
        }

        SDL_Log("Loaded keyboard profile \"%s\" from \"%s\"", name.c_str(), path.c_str());
        SDL_Log("DAS: %i, %i, %i", loaded->das, loaded->arr, loaded->dropArr);

        return loaded;
    }

    void InputProfile::save() {
        std::string path = PROFILE_DIRECTORY;
        path += name;
        path += FILE_EXTENSION;
        std::ofstream out(path, std::ios::trunc | std::ios::out);
        write(out);
        out.close();
        SDL_Log("Wrote keyboard profile \"%s\" to \"%s\"", name.c_str(), path.c_str());
    }

    void InputProfile::print() {
        std::cout << "Profile: " << name << '\n';
        write(std::cout);
    }

    void InputProfile::write(std::ostream &out) {
        out << das << '\n';
        out << arr << '\n';
        out << dropArr << '\n';

        out << T_rotation << '\n';
        out << O_rotation << '\n';
        out << L_rotation << '\n';
        out << J_rotation << '\n';
        out << Z_rotation << '\n';
        out << S_rotation << '\n';
        out << I_rotation << '\n';

        for (int i = 0; i < 8; i++)
            out << SDL_GetKeyName(SDL_GetKeyFromScancode(((SDL_Scancode) keys[i]))) << '\n';
    }

}