#pragma once

#include <Game/Board.h>
#include <Game/Piece.h>
#include <Game/TetrisEvent.h>
#include <Game/RotationTable.h>

#include <Game/Randomizers/XoroshiroRandomizer.h>

struct Game;
struct GameRules {
    virtual void on_update(Game* game, int dt) = 0;
    virtual void on_place(Game* game,int cleared, int combo)  = 0;
    virtual void on_win(Game* game) = 0;
    virtual void on_lose(Game* game) = 0;
    virtual int score(Game* game) = 0; 
    virtual bool result(Game* game) = 0;
};


struct Game {
    enum class State {
        Disabled,
        Playing,
        Results,
    };
    State state = State::Disabled;

    enum class Result {
        Lose,
        Win,
        AFK
    };
    Result result;

    struct Stats {
        int clears  = 0;
        int piecesPlaced  = 0;
        int lastPlacement = 0;
        int speed  = 0;
        int sent = 0;
    };
    Stats stats;

    Board board;    
    Piece heldPiece{0,0,0,0};
    Piece nextPiece{0,0,0,0};

    XoroshiroRandomizer pieceRandomizer;
    XoroshiroRandomizer holeRandomizer;

    RotationTable rotationTable{0,0,3,1,0,0,1};

    GameRules* rules = nullptr;

    int time = 0;
    int gravity_rate = 300;
	int gravityTimer = gravity_rate;

    int creep_rate = 0;
    int creep_timer;
    int last_creep = 0;
    bool creeping = false;

    int combo = 0;
    int comboTimer = 0;

    bool recieved_input;

    void Reset(int seed);
    void Update(int dt);

    void Events(TetrisEvent* events, int count);
    void Flip();
    void RCW();
    void RCCW();
    void Left();
    void Right();
    void SoftDrop();
    void SonicDrop();
    void HardDrop();
	void Win();
	void Lose();
    void Gravity();
	void Creep();
	void ApplyHeldPiece();

    inline void Clear(int count);

    // Helper Functions
    int GhostPieceY();


    const int INITIAL_PIECE_X = 3;
};


// Game Context API
// On_Win()
// On_Lose()
// On_Place()
// 