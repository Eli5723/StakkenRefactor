#pragma once

#include <UI/UI.h>
#include <UI/Node.h>

#include <Assets/Assets.h>
#include <Rendering/Renderer.h>

#include <HSV.h>

#include <Rendering/RenderGame.h>
#include <Data/Identity.h>

#include <Lobby.h>

namespace UI {

    struct Box : public Node {
        Box() {
            std::string test = "asdflkasjdf;lkasjdf;lkasjdf;laskdjf";
            size = Renderer::StrSize(0.5f, test, Assets::active_font);

            clickCallback = [](int, int) {
                SDL_Log("Hey!");
            };
        }

        void render() {

            std::string test = "asdflkasjdf;lkasjdf;lkasjdf;laskdjf";
            glm::vec2 strSize = Renderer::StrSize(0.5f, test, Assets::active_font);

            Renderer::DrawQuad(position, strSize, { 0,0,0,.9 });
            Renderer::DrawStr(position, 0.5f, test, Assets::active_font);
        }
    };

    struct Image : public Node {
        Assets::Texture* tex = 0;

        Image(const char* path) {
            tex = Assets::textures.get(path);
            if (tex)
                size = tex->size;
        }

        void render() {
            if (tex)
                Renderer::DrawQuad(position, size, tex->id);
            else
                Renderer::DrawQuad(position, size, { 255,0,220,1 });
        }
    };

    struct ImageBacked : public Node {
        Assets::Texture* tex = 0;

        ImageBacked(const char* path) {
            tex = Assets::textures.get(path);
            if (tex)
                size = tex->size;
        }

        void render() {
            if (tex) {
                Renderer::DrawQuad(position + glm::vec2{ 4.0f,4.0f }, size, tex->id, { 0,0,0,1 });
                Renderer::DrawQuad(position, size, tex->id);
            }
            else {
                Renderer::DrawQuad(position, size, { 255,0,220,1 });
            }
        }
    };

    const glm::vec2 margin = { 8,8 };

    struct TextButton : public Node {
        std::string text;

        TextButton(const char* text, const std::function<void(int, int)>& callback) : text(text) {
            size = Renderer::StrSize(0.5f, text, Assets::active_font) + margin * 2.0f;
            clickCallback = callback;
        }

        TextButton(const glm::vec2& size, const char* text, const std::function<void(int, int)>& callback) : text(text) {
            this->size = size;
            clickCallback = callback;
        }

        void render() {
            Renderer::DrawQuad(position, size, { 0,0,0,1 });
            Renderer::DrawStr(position + margin, 0.5f, text, Assets::active_font);
        }
    };

    // TODO: Add keyboard support to slider
    template<typename t>
    struct Slider : public Node {
        t& _val;
        t _max;
        t _min;
        std::string label;

        Slider(const glm::vec2& size, t _min, t _max, t& ref, const std::string& label) : _val(ref), label(label) {
            this->size = size;
            this->_max = _max;
            this->_min = _min;

            this->dragCallback = [this](const SDL_MouseMotionEvent& event) {
                this->_val = std::clamp(this->_val + (event.xrel / this->size.x), this->_min, this->_max);
            };
        }

        void render() {
            // Draw background and "bar"
            Renderer::DrawQuad(position, size, { 0,0,0,1 });
            Renderer::DrawQuad(position + glm::vec2{ 0,size.y / 4 * 3 - 4 }, { size.x,8 }, { .5,.5,.5,1 });

            Renderer::DrawStr(position, .5f, label.c_str(), Assets::active_font);
            // Draw cursors
            float a = (float)(_val - _min);
            float b = (float)(_max - _min);
            float cursor = (a / b) * size.x;

            Renderer::DrawQuad(position + glm::vec2{ cursor - 6,size.y / 4 * 3 - 6 }, { 12,12 }, { 1,1,1,1 });
        }
    };

    struct DraggableIcon : public Node {
        Assets::Texture* icon;
        DraggableIcon(const std::string& iconPath, const std::function<void(const SDL_MouseMotionEvent&)>& dragCallback) {
            this->icon = Assets::textures.get(iconPath.c_str());
            this->dragCallback = dragCallback;
            this->size = { 32,32 };
        }

        void render() {
            if (icon)
                Renderer::DrawQuad(position, size, icon->id);
        }
    };

    struct Button : public Node {
        Assets::Texture* texture = 0;

        Button(const std::string& path, int width, int height, const std::function<void(int, int)>& callback) {
            texture = Assets::textures.get(path.c_str());
            clickCallback = callback;
            size = { width,height };
        }

        Button(int width, int height, const std::function<void(int, int)>& callback) {
            clickCallback = callback;
            size = { width,height };
        }

        void render() {
            if (texture) {
                Renderer::DrawQuad(position, size, texture->id);
            }
            else
                Renderer::DrawQuad(position, size, { 1.0f, 1.0f,1.0f,1.0f });
        }
    };

    struct PieceEditor : public Node {
        int type;
        Identity& identity;
        float hue;
        float saturation;
        float value;

        PieceEditor(int type, Identity& identity) : type(type), identity(identity) {
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

        void render() {
            RenderGame::DrawPiece(position, &identity.colorTable, Assets::active_texture, type, identity.rotationTable[type]);
        }
    };

    struct ColorRotationEditor : public Node {
        Identity& identity;
        ColorRotationEditor(Identity& identity) : identity(identity) {
            for (int i = 0; i < 7; i++) {
                this->addChild(new PieceEditor(i, identity));
            }

            this->gridLayout(4);
        }

        void render() {
            Renderer::DrawQuad(position, size, { 0,0,0,1 });
        }
    };

    struct Checkbox : public Node {
        std::string label;
        int& ref;

        Assets::Texture* uncheckedTexture = Assets::textures.get("./resources/textures/checkbox.png");
        Assets::Texture* checkedTexture = Assets::textures.get("./resources/textures/checkedbox.png");

        Checkbox(const glm::vec2& size, int& ref, const std::string& label) : label(label), ref(ref) {
            this->size = size;
            clickCallback = [this](int, int) {this->ref = !this->ref;};
        }

        void render() {
            const glm::vec2 boxPosition = position + glm::vec2{ size.x - size.y,0 };

            Renderer::DrawQuad(position, size, { 0,0,0,1 });
            Renderer::DrawStr(position, 0.5f, label.c_str(), Assets::active_font);
            // Renderer::DrawQuad(boxPosition,{size.y, size.y},{1,1,1,1});

            Renderer::DrawQuad(boxPosition + glm::vec2{ 3,3 }, { size.y - 6, size.y - 6 }, ref ? checkedTexture->id : uncheckedTexture->id);
        }
    };

    const float GAME_OFFSET = (RenderGame::kPFPDimensions.x + RenderGame::MARGIN);

    struct GameViewer : public Node {
        Game* game;
        Identity* owner;
        float scale;

        GameViewer(Game* game, Identity* owner) : game(game), owner(owner) {
            hasStencil = true;
            clippedContents = true;
            scale = 1;
        }

        void stencil() {
            RenderGame::DrawBoardStencil(position + glm::vec2{ 0,GAME_OFFSET * scale }, scale);
        }

        void renderClipped() {
            RenderGame::DrawGame(position  + glm::vec2{ 0, GAME_OFFSET * scale }, *game, &owner->colorTable, Assets::active_texture, scale);
        }

        void render() {
            RenderGame::DrawGameUI(position  + glm::vec2{ 0, GAME_OFFSET * scale }, *game, &owner->colorTable, Assets::active_texture, scale);
            RenderGame::DrawHeader(position, owner->pfp, owner->name, scale);
        }
    };

    // RenderGame::DrawBoardStencil(pos, scale);
    // RenderGame::DrawGame(pos,*testBoard, &test, Assets::active_texture, scale);
    // RenderGame::DrawGameUI(pos,*testBoard, &test, Assets::active_texture, scale);
    // RenderGame::DrawHeader(pos - glm::vec2{0,64*scale}, testIdentity->pfp, testIdentity->name, scale);

}