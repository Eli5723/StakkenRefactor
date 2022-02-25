#include "Font.h"

#include <stb_truetype.h>
#include <SDL2/SDL_log.h>

#include <slurp.h>
#include <string>

// TODO: Improve packing algorthim, switch to SDF fonts

namespace Assets {

inline void stamp(u8* target, u8* source, int x, int y, int width, int height, int stride) {
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            target[(x + i) + (y + j) * stride] = source[i + j * width];
        }
    }
}

Font* Font::Load(const std::filesystem::path& path){
    unsigned const char* fontFile = slurpuc(path.c_str());

    if (!fontFile){
        SDL_LogWarn(0, "Failed to Load font file %s", path);
        return nullptr;
    }

    static stbtt_fontinfo fontInfo;
    if (!stbtt_InitFont(&fontInfo, fontFile,0)) {
        SDL_LogWarn(0, "Failed to initialize font file.");
        delete [] fontFile;
        return nullptr;
    }

    const int LINE_HEIGHT = 64;
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, LINE_HEIGHT);

    // Estimate Atlas Size
    int atlas_width = 0, atlas_height = 0, x0, x1, y0, y1, width, height;
    for (int charCode = 0; charCode < 255; charCode++){
        stbtt_GetCodepointBitmapBox(&fontInfo, charCode, scale, scale, &x0, &y0, &x1, &y1);
        int width = x1-x0;
        int height = y1-y0;
        atlas_width += width;
        atlas_height = atlas_height > height ? atlas_height : height;
    }

    if (atlas_width == 0 || atlas_height == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Failed to Load font: %s", path);
        return nullptr;
    }

    Font* newFont = new Font;

    // Vertical Metrics
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
    newFont->ascent = roundf(ascent * scale);
    newFont->descent = roundf(descent * scale);
    newFont->lineGap= roundf(lineGap * scale);
    newFont->lineHeight = LINE_HEIGHT;


    // Build atlas texture, calculate individual glyph metrics
    u8* atlasBitmap = new u8[atlas_width * atlas_height];

    int penX = 0;
    int offsetX, offsetY, advanceWidth, leftSideBearing;
    for (int charCode = 0; charCode < 255; charCode++){
        // Add Glyph to bitmap
        u8* bitmap = stbtt_GetCodepointBitmap(&fontInfo, scale, scale, charCode, &width, &height, &offsetX, &offsetY);
        stamp(atlasBitmap, bitmap, penX, 0, width, height, atlas_width);
        stbtt_FreeBitmap(bitmap,0);

        // Get Horizontal Metrics
        stbtt_GetCodepointHMetrics(&fontInfo, charCode, &advanceWidth, &leftSideBearing);

        Glyph& glyph = newFont->glyphs[charCode];
        glyph.advance = advanceWidth * scale;
        glyph.bearing = {leftSideBearing * scale, offsetY};
        glyph.size = {width, height};
        glyph.leftTop = {(float)penX/(float)atlas_width,0};
        glyph.rightTop = {(float)(penX+width)/(float)atlas_width,0};
        glyph.leftBottom = {(float)penX/(float)atlas_width,(float)height/(float)atlas_height};
        glyph.rightBottom = {(float)(penX+width)/(float)atlas_width,(float)height/(float)atlas_height};

        penX+= width;
    }      

    glGenTextures(1, &newFont->atlasId);
    glBindTexture(GL_TEXTURE_2D, newFont->atlasId);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, atlasBitmap);

    SDL_Log("Loaded Font!");
    delete[] atlasBitmap;
    delete [] fontFile;

    return newFont;
}
}