#pragma once

#include <UI/UI.h>

#include <UI/Widgets/PieceEditor.h>

namespace UI
{

struct ColorRotationEditor : public Node {
    Identity& identity;
    
    ColorRotationEditor(Identity& identity);
    void render();
};

}