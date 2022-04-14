#include <Rendering/RenderGame.h>

#include <Rendering/Renderer.h>

namespace RenderGame
{

    Settings settings;
    float pixelThickness = 1;

    // Draw the board; Ignore buffer overflows because the only possible consequence is extra overdraw.
    // [[clang::disable_sanitizer_instrumentation]]

    // __attribute__((clang::disable_sanitizer_instrumentation))
    void DrawBoard(const glm::vec2 &position, Board &board, ColorTable *colorTable, Assets::Texture *texture, float scale)
    {
        float tile_width = TILE_WIDTH * scale;
        glm::vec2 tile_dimensions = TILE_DIMENSIONS * scale;

        // Draw Tiles
        for (int y = Board::kOverflowRows - 1; y < Board::kTotalRows; y++)
        {
            for (int x = 0; x < Board::kWidth; x++)
            {
                int type = board.tileAt(x, y);
                if (type != TileType::Empty && type < TileType::Invalid)
                {

                    const glm::vec2 tilePosition = position + glm::vec2{tile_width * x, tile_width * (y - Board::kOverflowRows)};

                    if (texture)
                        Renderer::DrawQuad(tilePosition, tile_dimensions, texture->id, colorTable->entries[(int)type]);
                    else
                        Renderer::DrawQuad(tilePosition, tile_dimensions, colorTable->entries[(int)type]);

                    if (settings.outlineStyle == 1)
                    {
                        // Cultris Style Outlines
                        const Connection connections = board.connectionAt(x, y);
                        glm::vec4 outlineColor = colorTable->entries[(int)type] * glm::vec4{1.5f, 1.5f, 1.5, 1};
                        if ((connections & Side::Up) ^ Side::Up)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0, -1}, glm::vec2{tile_width + 1, 1}, outlineColor);

                        if ((connections & Side::Down) ^ Side::Down)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0, tile_width}, glm::vec2{tile_width, 1}, outlineColor);

                        if ((connections & Side::Left) ^ Side::Left)
                            Renderer::DrawQuad(tilePosition, glm::vec2{1, tile_width}, outlineColor);

                        if ((connections & Side::Right) ^ Side::Right)
                            Renderer::DrawQuad(tilePosition + glm::vec2{tile_width, 0}, glm::vec2{1, tile_width + 1}, outlineColor);
                    }
                    else if (settings.outlineStyle == 2)
                    {
                        // TGM Style Outlines
                        if (board.tileAt(x, y - 1) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0, -1}, glm::vec2{tile_width + 1, pixelThickness}, {1, 1, 1, 1});

                        if (y < Board::kTotalRows - 1 && board.tileAt(x, y + 1) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition + glm::vec2{0, tile_width}, glm::vec2{tile_width, pixelThickness}, {1, 1, 1, 1});

                        if (x > 0 && board.tileAt(x - 1, y) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition, glm::vec2{pixelThickness, tile_width}, {1, 1, 1, 1});

                        if (x < Board::kWidth && board.tileAt(x + 1, y) == TileType::Empty)
                            Renderer::DrawQuad(tilePosition + glm::vec2{tile_width, 0}, glm::vec2{pixelThickness, tile_width + 1}, {1, 1, 1, 1});
                    }
                }
            }
        }
    }

    void DrawBoardStencil(const glm::vec2 &position, float scale)
    {
        // GAMEPLAY
        Renderer::DrawQuad(position, BOARD_DIMENSIONS * scale, {0, 0, 0, settings.backgroundOpacity});
    }

    void DrawPiece(const glm::vec2 &position, Piece &piece, ColorTable *colorTable, Assets::Texture *texture, float scale = 1.0f)
    {
        float tilesize = TILE_WIDTH * scale;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (piece.tileAt(x, y) != TileType::Empty)
                {

                    glm::vec2 tilePosition = position + glm::vec2{tilesize * x, tilesize * y};

                    if (texture)
                        Renderer::DrawQuad(tilePosition, TILE_DIMENSIONS*scale, texture->id, colorTable->entries[piece.type]);
                    else
                        Renderer::DrawQuad(tilePosition, TILE_DIMENSIONS*scale, colorTable->entries[piece.type]);

                    const Connection connections = piece.connectionAt(x, y);
                    if ((connections & Side::Up) ^ Side::Up)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0, -1}, glm::vec2{tilesize, pixelThickness}, {1, 1, 1, 1});

                    if ((connections & Side::Down) ^ Side::Down)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0, tilesize - 1}, glm::vec2{tilesize, pixelThickness}, {1, 1, 1, 1});

                    if ((connections & Side::Left) ^ Side::Left)
                        Renderer::DrawQuad(tilePosition, glm::vec2{pixelThickness, tilesize}, {1, 1, 1, 1});

                    if ((connections & Side::Right) ^ Side::Right)
                        Renderer::DrawQuad(tilePosition + glm::vec2{tilesize - 1, 0}, glm::vec2{pixelThickness, tilesize}, {1, 1, 1, 1});
                }
            }
        }
    }

    // Draw a piece without having a piece object; used to draw pieces in the options menu
    void DrawPiece(const glm::vec2 &position, ColorTable *colorTable, Assets::Texture *texture, int type, int rotation)
    {
        Tile *tiles = PieceDefintions::getPieceTiles(type, rotation);
        Connection *connections = PieceDefintions::getPieceConnections(type, rotation);

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tiles[x + y * 4] != TileType::Empty)
                {
                    glm::vec2 tilePosition = position + glm::vec2{TILE_WIDTH * x, TILE_WIDTH * y};

                    if (texture)
                        Renderer::DrawQuad(tilePosition, TILE_DIMENSIONS, texture->id, colorTable->entries[type]);
                    else
                        Renderer::DrawQuad(tilePosition, TILE_DIMENSIONS, colorTable->entries[type]);

                    const Connection connection = connections[x + y * 4];
                    if ((connection & Side::Up) ^ Side::Up)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0, -1}, glm::vec2{TILE_WIDTH, pixelThickness}, {1, 1, 1, 1});

                    if ((connection & Side::Down) ^ Side::Down)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0, TILE_WIDTH - 1}, glm::vec2{TILE_WIDTH, pixelThickness}, {1, 1, 1, 1});

                    if ((connection & Side::Left) ^ Side::Left)
                        Renderer::DrawQuad(tilePosition, glm::vec2{pixelThickness, TILE_WIDTH}, {1, 1, 1, 1});

                    if ((connection & Side::Right) ^ Side::Right)
                        Renderer::DrawQuad(tilePosition + glm::vec2{TILE_WIDTH - 1, 0}, glm::vec2{pixelThickness, TILE_WIDTH}, {1, 1, 1, 1});
                }
            }
        }
    }

    void DrawGhostPiece(const glm::vec2 &position, Piece &piece, ColorTable *colorTable, Assets::Texture *texture, float scale = 1.0f)
    {
        float tilesize = TILE_WIDTH * scale;

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (piece.tileAt(x, y) != TileType::Empty)
                {
                    const glm::vec2 tilePosition = position + glm::vec2{tilesize * x, tilesize * y};
                    // Because the background of the board is black, we can get away with tinting the ghost piece a darker shade to "cheat" the transparency. This feels better when the background of the board is not opaque
                    if (texture)
                        Renderer::DrawQuad(tilePosition, TILE_DIMENSIONS*scale, texture->id, colorTable->entries[piece.type] * glm::vec4{.5f, .5f, .5f, 1.0f});
                    else
                        Renderer::DrawQuad(tilePosition, TILE_DIMENSIONS*scale, colorTable->entries[piece.type] * glm::vec4{.5f, .5f, .5f, 1.0f});
                    ;

                    if (!settings.outlineGhostPiece)
                        continue;

                    const Connection connection = piece.connections[x + y * 4];
                    if ((connection & Side::Up) ^ Side::Up)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0, -1}, glm::vec2{tilesize, pixelThickness}, {1, 1, 1, .3f});

                    if ((connection & Side::Down) ^ Side::Down)
                        Renderer::DrawQuad(tilePosition + glm::vec2{0, tilesize - 1}, glm::vec2{tilesize, pixelThickness}, {1, 1, 1, .3f});

                    if ((connection & Side::Left) ^ Side::Left)
                        Renderer::DrawQuad(tilePosition, glm::vec2{pixelThickness, tilesize}, {1, 1, 1, .3f});

                    if ((connection & Side::Right) ^ Side::Right)
                        Renderer::DrawQuad(tilePosition + glm::vec2{tilesize - 1, 0}, glm::vec2{pixelThickness, tilesize}, {1, 1, 1, .3f});
                }
            }
        }
    }

    void DrawPiecePreview(const glm::vec2 &position, Game &game, ColorTable* colorTable, Assets::Texture* texture,  float scale){
        Renderer::DrawQuad(position, PIECE_DIMENSIONS*scale, {0.0f,0.0f,0.0f, settings.backgroundOpacity});
        Renderer::QuadBox(position, PIECE_DIMENSIONS*scale, pixelThickness, {1,1,1,1});
        DrawPiece(position, game.nextPiece, colorTable, texture, scale);
    }

    void DrawStats(const glm::vec2 &position, Game &game, float scale){
        
        const glm::vec2 size = {PIECE_DIMENSIONS.x, BOARD_DIMENSIONS.y - PIECE_DIMENSIONS.y - PADDING};
        
        Renderer::DrawQuad(position, size*scale, {0.0f,0.0f,0.0f, settings.backgroundOpacity});
        Renderer::QuadBox(position, size*scale, pixelThickness, {1,1,1,1});
        
        const Game::Stats& stats = game.stats;

        
        if (scale <= .3f)
            return;


        char textBuffer[75];
        float bpm = (float)stats.piecesPlaced/((float)game.time/60000.0f);
        bpm += 0.00001f;

        float seconds = (float)game.time/1000.0f;
        sprintf(textBuffer,"Time:\n%.2f\nCombo:\n%i (%i)\n Pending:\n%i\nCleared:\n%i", seconds, game.combo, game.stats.maxCombo, game.pending_lines, game.stats.clears); // game.pendingLines
        Renderer::DrawStr(position, .5f * scale, textBuffer, Assets::active_font);
        
        float comboRem = std::lerp(0.0f,1.0f,game.comboTimer / 2400.0f);
        comboRem = comboRem > 0.0f ? comboRem : 0.0f;
        comboRem = comboRem < 1.0f ? comboRem : 1.0f;   
        Renderer::DrawQuad(position, {kStatsDimensions.x * comboRem, kStatsDimensions.y},{1.0f,0,0,.5});
    
        sprintf(textBuffer,"%.0f",bpm);
        Renderer::DrawNeedle(position + glm::vec2{kStatsDimensions.x/2,kStatsDimensions.y - 100}*scale, 55.0f, glm::radians(180.0f + (bpm)));
        Renderer::DrawStrC(position + glm::vec2{kStatsDimensions.x/2 -10, kStatsDimensions.y - 100}*scale, 2.0f*scale, textBuffer, Assets::active_font);
        // DrawPiece(position, game.nextPiece, colorTable, texture);
    }

    #define DIGIT_STR(digit) {(char)('0' + digit), '\0'}

    void DrawGame(const glm::vec2 &position, Game &game, ColorTable *colorTable, Assets::Texture *texture, float scale)
    {
        DrawBoard(position, game.board, colorTable, texture, scale);
        int ghostY = game.GhostPieceY();
        DrawGhostPiece(position + glm::vec2(game.heldPiece.x * TILE_WIDTH * scale, (ghostY - Board::kOverflowRows) * TILE_WIDTH * scale), game.heldPiece, colorTable, texture, scale);

        // Draw Actual Piece after ghost piece to prevent blending
        DrawPiece(position + glm::vec2(game.heldPiece.x * TILE_WIDTH, (game.heldPiece.y - Board::kOverflowRows) * TILE_WIDTH), game.heldPiece, colorTable, texture, scale);


        if (game.time < 0) {
            int second = abs(game.time / 1000) + 1;
            float sscale = 4.0f - ((game.time%1000)/1000.0f)*5 * scale;
            char str[2] DIGIT_STR(second);
            glm::vec2 size = Renderer::StrSize(sscale, (char*)&str, Assets::active_font);
            Renderer::DrawStr(position + glm::vec2{BOARD_DIMENSIONS.x*.5f, BOARD_DIMENSIONS.y*.3f} - (size*.5f), sscale, (char*)&str, Assets::active_font);
        }

    }

    void DrawGameUI(const glm::vec2 &position, Game &game, ColorTable* colorTable, Assets::Texture* texture, float scale){

        // Border
        Renderer::QuadBox(position, BOARD_DIMENSIONS * scale, pixelThickness, {1,1,1,1});
        glm::vec2 status_offset = BOARD_DIMENSIONS * scale * glm::vec2{1.0f / 2.0f, 1.0f / 3.0f};
    
        DrawPiecePreview(position + glm::vec2{BOARD_DIMENSIONS.x + PADDING, 0.0f} * scale, game, colorTable, texture, scale);
        DrawStats(position + glm::vec2{BOARD_DIMENSIONS.x + PADDING, PIECE_DIMENSIONS.y + PADDING} * scale, game, scale);
    
        // Draw Countdown if the game is being played and the time is less than 0
        if (game.state == Game::State::Playing){
            if (game.time > 0)
                return;

            return;
        }

        // Dim the board and draw a message explaning why it's paused
        Renderer::DrawQuad(position, BOARD_DIMENSIONS * scale, {0, 0, 0, .3f});

        if (game.state == Game::State::Disabled)
        {
            status_offset = BOARD_DIMENSIONS * scale * glm::vec2{1.0f / 2.0f, 1.0f / 3.0f};
            Renderer::DrawStrC(position + status_offset, 2.0f*scale, "Paused", Assets::active_font);
        }

        if (game.state == Game::State::Results)
        {
            switch (game.result){
            break; case Game::Result::Win:
                Renderer::DrawStrC(position + status_offset, 2.0f*scale, "You Win!", Assets::active_font);
            break; case Game::Result::Lose:
                Renderer::DrawStrC(position + status_offset, 2.0f*scale, "You Lose!", Assets::active_font);
            break; case Game::Result::AFK:
                Renderer::DrawStrC(position + status_offset, 2.0f*scale, "Afk...!", Assets::active_font);
            }
        }
    }


    const float FSCALE = (48.0f / 64.0f);
    void DrawHeader(const glm::vec2 &position, Assets::Texture* pfp, const std::string& name, float scale){
        Renderer::DrawQuad(position, kPFPDimensions * scale, pfp->id, {1,1,1,1});
        // float namescale = Renderer::StrSize(1.0f, name, Assets::active_font).x / RenderGame::BOARD_DIMENSIONS.x;
        
        Renderer::DrawStr(position + glm::vec2{64 - RenderGame::MARGIN - 1, 1} * scale, FSCALE * scale, name.c_str(), Assets::active_font, {0,0,0,1});
        Renderer::DrawStr(position + glm::vec2{64 - RenderGame::MARGIN, 0} * scale, FSCALE * scale, name.c_str(), Assets::active_font);
    }

}