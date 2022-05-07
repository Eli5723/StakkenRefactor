#include "Font.h"

#include <types.h>

#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>

using namespace msdfgen;

struct Character {
    vec2 size;
    vec2 bearing;
    vec2 origin;
    float advance;
};


bool Init() {
    // Initialize Freetype
    FreetypeHandle* ft = initializeFreetype();
    if (!ft) 
        return false;

    // Load Font
    FontHandle* font = loadFont(ft, "C:\\Windows\\Fonts\\arialbd.ttf");
    if (!font)
        return false;
    
    // Prepare Buffer for rendering
    unsigned int tex_width = 1024;
    unsigned int tex_height = tex_width;
    unsigned int buffer_size = tex_width * tex_height * 3;
    uint8_t* pixels = new uint8_t[buffer_size]{0};
    unsigned int pen_x = 0;
    unsigned int pen_y = 0;
    int max_height = 0;

    // Load Metrics
    FontMetrics metrics;
    getFontMetrics(metrics, font);

    msdfgen::Shape shape;
    msdfgen::Shape::Bounds bounds;

    float glyph_width = 0;
    float glyph_height = 0;
    double advance = 0;
    float border_width = 4;

    for (int charcode = 0; charcode < 128; charcode++){
        msdfgen::loadGlyph(shape, font, charcode, &advance);
    
        
        if (shape.validate() && shape.contours.size() > 0)
        {
            shape.normalize();
            shape.inverseYAxis = true;

            bounds = shape.getBounds(border_width);

            // Calculate width & height
            glyph_width = ceil(bounds.r - bounds.l);
            h = ceil(bounds.t - bounds.b);

            if (max_height < glyph_height)
                max_height = glyph_height;

            msdfgen::edgeColoringSimple(shape, 3.0);
            msdfgen::Bitmap<float, 3> msdf(glyph_width, glyph_height);
            msdfgen::generateMSDF(msdf, shape, border_width, 1.0, msdfgen::Vector2(-bounds.l, -bounds.b));

            if (pen_x + msdf.width() >= tex_width)
            {
                pen_x = 0;
                pen_y += max_height;
                max_height = 0;
            }

            for (unsigned int row = 0; row < msdf.height(); ++row)
            {
                for (unsigned int col = 0; col < msdf.width(); ++col)
                {
                    int x = pen_x + col;
                    int y = pen_y + row;

                    pixels[(x + tex_width * y) * 4 + 0] =
                        msdfgen::pixelFloatToByte(msdf(col, row)[0]);
                    pixels[(x + tex_width * y) * 4 + 1] =
                        msdfgen::pixelFloatToByte(msdf(col, row)[1]);
                    pixels[(x + tex_width * y) * 4 + 2] =
                        msdfgen::pixelFloatToByte(msdf(col, row)[2]);
                    pixels[(x + tex_width * y) * 4 + 3] = 255;
                }
            }
        }

        Character character = {
            vec2(glyph_width, glyph_height),
            vec2((float)(bounds.l), (float)(bounds.t)),
            vec2((float)(pen_x), (float)(pen_y)), 
            advance
        };

    }





    if (font) {
        Shape shape;
        if (loadGlyph(shape, font, 'A')) {
            shape.normalize();
            //                      max. angle
            edgeColoringSimple(shape, 3.0);
            //           image width, height
            Bitmap<float, 3> msdf(32, 32);
            //                     range, scale, translation
            generateMSDF(msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0));
            savePng(msdf, "output.png");
        }
        destroyFont(font);
    }
    deinitializeFreetype(ft);

    return false;
}

bool generateAtlas(const char *fontFilename) {

}