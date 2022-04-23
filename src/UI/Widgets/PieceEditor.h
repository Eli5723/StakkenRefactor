#pragma once

#include <UI/UI.h>

namespace UI
{

struct PieceEditor : public Node {
    int type;
    Identity& identity;
    float hue;
    float saturation;
    float value;

    PieceEditor(int type, Identity& identity);
    void render();
};

}