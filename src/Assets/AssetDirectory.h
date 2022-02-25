#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

namespace Assets {

    using ExtensionList = std::vector<std::string>;

    class AssetDirectory {
        public:

        std::string label;
        std::filesystem::path directory;

        const ExtensionList validExtensions;

        std::vector<std::filesystem::path> files;
        std::vector<std::string> names;

        // Used for cycling through options
        int position = 0;

        AssetDirectory(const ExtensionList& extensions, const std::filesystem::path& directory, const std::string& label) : label(label), directory(directory), validExtensions(extensions), files(), names() {
        }

        void scan() {
            SDL_Log("Scanning for %s", label.c_str());
            files.clear();
            names.clear();

            for (auto& p : std::filesystem::directory_iterator(directory)) {
                std::filesystem::path file = p.path();

                if (!valid(file))
                    continue;

                SDL_Log("	%s", file.c_str());

                std::string name = p.path().stem().string();
                files.push_back(p.path());
                names.push_back(name);
            }
        }

        bool valid(const std::filesystem::path& file) {
            std::filesystem::path fileExtension = file.extension();

            if (std::find(validExtensions.begin(), validExtensions.end(), fileExtension) != validExtensions.end())
                return true;

            return false;
        }


        std::filesystem::path& atIndex(int index) {
            if (index < 0)
                index = files.size() - 1;
            
            return files[index % files.size()];
        }

        std::filesystem::path& next(){
            position++;
            return atIndex(position);
        }

        std::filesystem::path& previous(){
            position--;

            if (position < 0)
                position = files.size() - 1;
            

            return atIndex(position);
        }
    };

}