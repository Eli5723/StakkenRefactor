#include <UI/Widgets/ColorRotationEditor.h>

namespace UI
{

ColorRotationEditor::ColorRotationEditor(Identity& identity) : identity(identity) {
    for (int i = 0; i < 7; i++) {
        this->addChild(new PieceEditor(i, identity));
    }

    this->gridLayout(4);
}

void ColorRotationEditor::render() {
    Renderer::DrawQuad(position, size, { 0,0,0,1 });
}

}