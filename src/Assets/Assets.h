#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>

#include <Assets/Texture.h>
#include <Assets/BGShader.h>
#include <Assets/Sound.h>
#include <Assets/Font.h>

#include <Assets/AssetDirectory.h>

namespace Assets {

using ExtensionList = std::vector<std::string>;

const ExtensionList ImageExtensions = {".png",".jpg"};
const ExtensionList ShaderExtensions = { ".frag" };
const ExtensionList FontExtensions = { ".ttf" };

template <class type>
struct AssetCache {
	std::unordered_map<std::string, type*> cached_items;

	// Load file if it hasn't been loaded already, otherwise return the cached item
    // TODO: Flush unused cache items? probably not needed
	type* get(const std::filesystem::path& path) {

		auto iter = cached_items.find(path.string());

		if (iter != cached_items.end()){
            return iter->second;
        }
			        
        type* loaded = type::Load(path);
        
        if (!loaded) {
            return nullptr;
        }
        
        cached_items[path] = loaded;
        
        return loaded;
	}
};

extern AssetCache<Texture> textures;
extern AssetCache<BGShader> bgShaders;
extern AssetCache<Sound> sounds;
extern AssetCache<Font> fonts;

extern AssetDirectory textureDirectory;
extern AssetDirectory shaderDirectory;
extern AssetDirectory FontDirectory;

extern BGShader* active_shader;
extern Texture* active_texture;
extern Font* active_font;

void Init();

}
