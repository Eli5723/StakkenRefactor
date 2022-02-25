#include <Assets/Assets.h>

namespace Assets {

using ExtensionList = std::vector<std::string>;

AssetCache<Texture> textures;
AssetCache<BGShader> bgShaders;
AssetCache<Sound> sounds;
AssetCache<Font> fonts;

AssetDirectory textureDirectory(ImageExtensions, "./resources/pieceTextures/", "Textures");
AssetDirectory shaderDirectory(ShaderExtensions, "./resources/shaders/", "Shaders");
AssetDirectory fontDirectory(FontExtensions, "./resources/fonts/", "Fonts");

BGShader* active_shader = 0;
Texture* active_texture = 0;
Font* active_font = 0;

void Init(){
    Sound::Init();
    BGShader::Init();

    textureDirectory.scan();
    shaderDirectory.scan();
    fontDirectory.scan();

}

}
