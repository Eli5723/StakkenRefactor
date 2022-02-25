#pragma once

#include <glm/glm.hpp>

#include <Assets/Assets.h>

#include <Game/Game.h>
#include <Game/Board.h>
#include <Game/Piece.h>

#include <Data/ColorTable.h>

namespace RenderGame
{

    const float MARGIN = 2.0f;
    const float PADDING = 16.0f;

    const float TILE_WIDTH = 32.0f;

    const glm::vec2 TILE_DIMENSIONS{TILE_WIDTH, TILE_WIDTH};                                  // 16 x 16
    const glm::vec2 PIECE_DIMENSIONS{TILE_WIDTH * 4, TILE_WIDTH * 4};                         // 64 x 64
    const glm::vec2 BOARD_DIMENSIONS{TILE_WIDTH * Board::kWidth, TILE_WIDTH *Board::kHeight}; // 160 x 288
    const glm::vec2 GAME_DIMENSIONS{BOARD_DIMENSIONS.x + PADDING + PIECE_DIMENSIONS.x, BOARD_DIMENSIONS.y};

    const glm::vec2 PLAYER_DIMENSIONS{GAME_DIMENSIONS.x, BOARD_DIMENSIONS.y + 48.0f + MARGIN};

    const glm::vec2 kPreviewOffset = glm::vec2{BOARD_DIMENSIONS.x + PADDING, 0};
    const glm::vec2 kStatsDimensions = glm::vec2{PIECE_DIMENSIONS.x, BOARD_DIMENSIONS.y - (PADDING + PIECE_DIMENSIONS.y)};

    const float kProfilePictureSize = 48.0f;
    const glm::vec2 kPFPDimensions{kProfilePictureSize, kProfilePictureSize};
    const glm::vec2 kIdentityDimensions = glm::vec2{kProfilePictureSize, kProfilePictureSize + PADDING * 2};
    const glm::vec2 kGameDimensions = BOARD_DIMENSIONS + glm::vec2{kStatsDimensions.x + PADDING * 4, kIdentityDimensions.y};


    struct Settings
    {
        int outlineStyle = 2;
        float backgroundOpacity = .85f;
        bool outlineGhostPiece;
    };

    extern Settings settings;

    void DrawBoard(const glm::vec2 &position, Board &board, ColorTable *colorTable, Assets::Texture *texture, float scale = 1);
    void DrawBoardStencil(const glm::vec2 &position, float scale = 1.0f);
    void DrawPiece(const glm::vec2 &position, Piece &piece, ColorTable *colorTable, Assets::Texture *texture);

    // Draw a piece without having a piece object; used to draw pieces in the options menu
    void DrawPiece(const glm::vec2 &position, ColorTable *colorTable, Assets::Texture *texture, int type, int rotation);
    void DrawGhostPiece(const glm::vec2 &position, Piece &piece, ColorTable *colorTable, Assets::Texture *texture);
    void DrawPiecePreview(const glm::vec2 &position, Game &game, ColorTable* colorTable, Assets::Texture* texture,  float scale = 1.0f);
    void DrawStats(const glm::vec2 &position, Game &game, float scale = 1.0f);
    void DrawGame(const glm::vec2 &position, Game &game, ColorTable *colorTable, Assets::Texture *texture, float scale = 1.0f);
    void DrawGameUI(const glm::vec2 &position, Game &game, ColorTable* colorTable, Assets::Texture* texture, float scale = 1.0f);

    void DrawHeader(const glm::vec2 &position, Assets::Texture* pfp, const std::string& name, float scale);

}