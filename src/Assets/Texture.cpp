#include <Assets/Texture.h>

#include <stb_image.h>
#include <SDL2/SDL.h>

namespace Assets {

Texture* Texture::Load(const std::filesystem::path& path){
    int width, height, nrChannels;
	unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

    if (!imageData){
        SDL_Log("Failed to load texture: '%s'", path.c_str());
        return nullptr;
    }

    GLuint id;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
    
    Texture* newTex = new Texture;

    newTex->id = id;
    newTex->name = path.stem();
    newTex->size = {width,height};
    newTex->path = path;

    return newTex;
}

Texture* Texture::Load(const std::filesystem::path& path, GLuint filterMode){
    int width, height, nrChannels;
	unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

    if (!imageData){
        SDL_Log("Failed to load texture: '%s'", path.c_str());
        return nullptr;
    }

    GLuint id;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
    
    Texture* newTex = new Texture;

    newTex->id = id;
    newTex->name = path.stem();
    newTex->size = {width,height};
    newTex->path = path;

    return newTex;
}


}