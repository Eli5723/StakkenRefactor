#include <UI/Widgets/PieceEditor.h>

namespace UI
{

PieceEditor::PieceEditor(int type, Identity& identity) : type(type), identity(identity) {
        this->size = { 128,164 };
        // Load original values
        RGB2HSV(hue, saturation, value, identity.colorTable.entries[type]);

        auto hueIcon = new UI::DraggableIcon("./resources/textures/hue.png", [this](const SDL_MouseMotionEvent& mot) {
            hue = hue + mot.yrel;
            HSV2RGB(hue, saturation, value, this->identity.colorTable.entries[this->type]);
            });
        hueIcon->offset = { 0,128 };
        this->addChild(hueIcon);

        auto satIcon = new UI::DraggableIcon("./resources/textures/saturation.png", [this](const SDL_MouseMotionEvent& mot) {
            saturation = fmax(0, fmin(1, saturation - mot.yrel * .025));
            HSV2RGB(hue, saturation, value, this->identity.colorTable.entries[this->type]);
            });
        satIcon->offset = { 32,128 };
        this->addChild(satIcon);

        auto valIcon = new UI::DraggableIcon("./resources/textures/value.png", [this](const SDL_MouseMotionEvent& mot) {
            value = fmax(0, fmin(1, value - mot.yrel * .025));
            HSV2RGB(hue, saturation, value, this->identity.colorTable.entries[this->type]);
            });
        valIcon->offset = { 64,128 };
        this->addChild(valIcon);

        auto rotIcon = new UI::Button("./resources/textures/rcw.png", 32, 32, [this](int, int) {
            this->identity.rotationTable[this->type] = (this->identity.rotationTable[this->type] + 1) % 4;
            });
        rotIcon->offset = { 96,128 };
        this->addChild(rotIcon);
        this->ApplyOffsets();
}

void PieceEditor::render() {
    RenderGame::DrawPiece(position, &identity.colorTable, Assets::active_texture, type, identity.rotationTable[type]);
}

}