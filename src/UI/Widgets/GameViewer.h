#pragma once

#include <UI/UI.h>

namespace UI
{

const float GAME_OFFSET = (RenderGame::kPFPDimensions.x + RenderGame::MARGIN);

struct GameViewer : public Node {
    Game* game = 0;
    Identity* owner = 0;
    float scale = 1;

    GameViewer() {
        hasStencil = true;
        clippedContents = true;
        scale = 1;
    }

    GameViewer(Game* game, Identity* owner) : game(game), owner(owner) {
        hasStencil = true;
        clippedContents = true;
        scale = 1;
    }

    void stencil() {
        if (game)
        RenderGame::DrawBoardStencil(position + glm::vec2{ 0,GAME_OFFSET * scale }, scale);
    }

    void renderClipped() {
        if (game)
        RenderGame::DrawGame(position  + glm::vec2{ 0, GAME_OFFSET * scale }, *game, &owner->colorTable, Assets::active_texture, scale);
    }

    void render() {
        if (game)
        RenderGame::DrawGameUI(position  + glm::vec2{ 0, GAME_OFFSET * scale }, *game, &owner->colorTable, Assets::active_texture, scale);
        
        if (owner)
        RenderGame::DrawHeader(position, owner->pfp, owner->name, scale);
    }
};

}