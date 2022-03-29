#include <Game/Game.h>

#include <Assets/Sounds.h>

// Constants
int ClearGarbageTable[]{
    0,0,1,2,3
};

int ComboTimeTable[]{
    2400, 1100, 400, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int ComboGarbageTable[]{
    0, 1, 1, 1, 2, 2, 3, 5, 7, 9, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
};

int ComboBonusTimeTable[]{
    1200, 600, 300, 200, 100, 0, 0, 0, 0, 0, 0, 0, -100, -200, -300, -600, -1200, -1200
};


void Game::Reset(int seed){
    pieceRandomizer.seed(seed);
    holeRandomizer.seed(seed);

    // Can't start with S or Z
    TileType firstPieceType = (TileType)(pieceRandomizer.next()%7);
    while (firstPieceType == TileType::S || firstPieceType == TileType::Z)
        firstPieceType = (TileType)(pieceRandomizer.next()%7);

    TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);

    heldPiece.Convert(INITIAL_PIECE_X, 0, firstPieceType, rotationTable[firstPieceType]);
    nextPiece.Convert(INITIAL_PIECE_X, 0, nextPieceType, rotationTable[nextPieceType]);

    state = Game::State::Playing;

    stats = {0};
    time = 0;
    comboTimer = 0;
    combo = 0;  
  
    creep_timer = 0;

    board.clear();

    if (rules)
        rules->on_init(this);
}

void Game::Update(int dt){
    if (state != Game::State::Playing)
        return;

    if (rules){
        rules->on_update(this, dt);
    }

    time += dt;

    gravityTimer -= dt;
    while (gravityTimer <= 0)
    {
        Gravity();
        gravityTimer += gravity_rate;
    }

    comboTimer -= dt;
}


void Game::Flip(){
    heldPiece.flip();
    if (!board.offsetTest(&heldPiece))
        heldPiece.flip();
}

void Game::RCW()
{
    heldPiece.rotateCW();
    if (!board.offsetTest(&heldPiece))
        heldPiece.rotateCCW();
}

void Game::RCCW()
{
    heldPiece.rotateCCW();
    if (!board.offsetTest(&heldPiece))
        heldPiece.rotateCW();
}

void Game::Left()
{
    heldPiece.moveLeft();
    if (!board.checkFit(&heldPiece))
    {
        heldPiece.moveRight();
    }
}

void Game::Right()
{
    heldPiece.moveRight();
    if (!board.checkFit(&heldPiece))
        heldPiece.moveLeft();

}

void Game::SoftDrop()
{
    heldPiece.moveDown();
    if (!board.checkFit(&heldPiece))
    {
        heldPiece.moveUp();
    }
    else
    {
        gravityTimer = gravity_rate;
    }
}

void Game::SonicDrop()
{
    int distance = 0;

    while (board.checkFit(&heldPiece))
    {
        heldPiece.moveDown();
        distance++;
    }
    heldPiece.moveUp();

    // Prevent stalling
    if (distance > 1)
    {
        gravityTimer = gravity_rate;
    }
}

void Game::HardDrop()
{
    while (board.checkFit(&heldPiece))
        heldPiece.moveDown();
    heldPiece.moveUp();
    ApplyHeldPiece();
}

void Game::Creep(){
    if (heldPiece.y > 0)
        heldPiece.moveUp();
    board.addLine(holeRandomizer.next() % 10);
}

void Game::Gravity()
{
    heldPiece.moveDown();
    if (!board.checkFit(&heldPiece))
    {
        heldPiece.moveUp();
        ApplyHeldPiece();
    }
}


void Game::ApplyHeldPiece(){
    // If the piece cannot possibly be applied, lose
    if (!board.checkFit(&heldPiece)) {
        Lose();
        return;
    }
    // Apply Piece to board
    int cleared = board.ApplyPiece(&heldPiece);

    // Get next piece
    heldPiece.Convert(INITIAL_PIECE_X, 0, nextPiece.type, rotationTable[nextPiece.type]);

    TileType nextPieceType = (TileType)(pieceRandomizer.next()%7);
    nextPiece.Convert(INITIAL_PIECE_X, 0, nextPieceType, rotationTable[nextPieceType]);

    if (cleared > 0){
        Clear(cleared);
    } else {
        comboTimer -= 400;
    }

    Sounds::play(Sounds::Slot::Lock);
}

void Game::Clear(int count){
    // Report Cleared Lines to Game Rules
    stats.clears += count;
    
     if (comboTimer < 0)
    {
        combo = 0;
        comboTimer = ComboTimeTable[0];
    }
    else
    {
        comboTimer += ComboTimeTable[combo];
        comboTimer += count * ComboBonusTimeTable[combo];
    }
    
    int send = 0;
    send += ClearGarbageTable[count];
    send += ComboGarbageTable[combo];
    combo++;

    if (combo > stats.maxCombo)
        stats.maxCombo = combo;

    Sounds::play(Sounds::Slot::Clear);

    if (combo == 5)
    {
        Sounds::play(Sounds::Slot::Combo5);
    }
    else if (combo == 7)
    {
        Sounds::play(Sounds::Slot::Combo7);
    }
    else if (combo == 10)
    {
        Sounds::play(Sounds::Slot::Combo10);
    }
    else if (combo == 12)
    {
        Sounds::play(Sounds::Slot::Combo12);
    }
    else if (combo == 14)
    {
        Sounds::play(Sounds::Slot::Combo14);
    }

    stats.sent += send;
    if (rules){
        rules->on_place(this, count, send);
    }
}

void Game::Win(){
    state = Game::State::Results;
    result = Game::Result::Win;

    if (rules){
        rules->on_win(this);
    }
}

void Game::Lose(){
    state = Game::State::Results;
    result = Game::Result::Lose;

    if (rules){
        rules->on_lose(this);
    }
}

void Game::Events(TetrisEvent* events, int count){
    if (state != Game::State::Playing)
        return;

    for (int i = 0; i < count; i++){
        const TetrisEvent e = events[i];

        // TODO: REPLAY record events
        switch (e){
            case TetrisEvent::Left:
                Left();
            break;

            case TetrisEvent::Right:
                Right();
            break;

            case TetrisEvent::SoftDrop:
                SoftDrop();
            break;

            case TetrisEvent::SonicDrop: {
                SonicDrop();
            } break;

            case TetrisEvent::HardDrop:
                HardDrop();
            break;

            case TetrisEvent::Flip:
                Flip();
            break;

            case TetrisEvent::RCW:
                RCW();
            break;
            case TetrisEvent::RCCW:
                RCCW();
            break;

            // Game Events
            case TetrisEvent::Gravity:
                Gravity();
            break;
            case TetrisEvent::Creep:
                Creep();
                break;
            
            case TetrisEvent::None:
            default:
            break;
        }
    }
}

int Game::GhostPieceY()
{
    int oldY = heldPiece.y;

    while (board.checkFit(&heldPiece))
        heldPiece.moveDown();
    heldPiece.moveUp();

    int ghostY = heldPiece.y;
    heldPiece.y = oldY;

    return ghostY;
}