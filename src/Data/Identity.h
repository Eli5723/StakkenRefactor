#pragma once

#include <Data/ColorTable.h>
#include <Game/RotationTable.h>

#include <string>
#include <Assets/Assets.h>


#define rgbToFloat(r,g,b) {(float)r/255.0f,(float)g/255.0f,(float)b/255.0f,1}

struct Identity {
    std::string name;
    Assets::Texture* pfp;
    
    ColorTable colorTable {.entries{
        rgbToFloat(237,41,57), //T
        rgbToFloat(255,121,0), //O
        rgbToFloat(254,203,0), //L 
        rgbToFloat(105,190,40), //J
        rgbToFloat(0,159,218), //Z
        rgbToFloat(0,101,189), //S
        rgbToFloat(149,45,152),  //I
        rgbToFloat(40,40,40)
        }
    };
    RotationTable rotationTable;

    static Identity* Load(const std::string& name);
    static Identity* LoadRandom();
	void save();
    void decode(std::istream& input);
	void encode(std::ostream& out);
};