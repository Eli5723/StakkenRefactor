#include <States/ChallengePlayState.h>

#include <Application.h>
#include <UI/UI.h>

#include <States/ChallengeSelectState.h>

#include <Game/Game.h>
#include <Challenges/Challenges.h>

UI::Node* exitButton;

UI::Node* challengeGameViewer;

UI::Node* gamemodeInformation;

Game* board;

void ChallengePlayState::init(){
    // Exit Button
    exitButton = new UI::TextButton("Back", [](int,int){
        Application::instance->state_set(new ChallengeSelectState);
    });
    UI::AddToScreen(exitButton);

    // Game
    board = new Game;
    board->rules = Challenges::getChallenge(challengeID);
    
    board->demo = new Recorder;
    board->Reset(clock());

    challengeGameViewer = new UI::GameViewer(board, Application::instance->local_identity);
	challengeGameViewer->position = {RenderGame::PADDING, UI::Resolution().y - RenderGame::PLAYER_DIMENSIONS.y - RenderGame::PADDING};
    UI::AddToScreen(challengeGameViewer);

    // Game Objective
    gamemodeInformation = new UI::Container;
    gamemodeInformation->addChild(new UI::Label(Challenges::getChallengeName(challengeID)));
    gamemodeInformation->addChild(new UI::Label(Challenges::getChallengeDescription(challengeID)));

    char recordText[40] = "Record:\n                              ";
    int record = Challenges::getRecord(challengeID);
    board->rules->score_format(record, &recordText[10]);

    gamemodeInformation->addChild(new UI::Label(recordText));
    
    UI::AddToScreen(gamemodeInformation);
    gamemodeInformation->listLayout();
    gamemodeInformation->right();


    board->on_win = [](){
        printf("The challenge was won!\n");
    };

}

void ChallengePlayState::update(int dt, int time){
    EventBuffer* events = Application::instance->local_controller->getEvents();
	board->Events(events->events, events->count);

	board->Update(dt);
}

void ChallengePlayState::render(int dt, int time){

}

bool ChallengePlayState::key_capture(const SDL_KeyboardEvent& key){
    if (key.keysym.sym == SDLK_F2){
        board->Reset(clock());
        return true;
    }

    if (key.keysym.sym == SDLK_F3){
        board->Disable();
        return true;
    }

    return false;
}

void ChallengePlayState::close(){
    exitButton->destroy_recursive();
    challengeGameViewer->destroy();

    delete board->demo;
    delete board;

    gamemodeInformation->destroy_recursive();

    UI::ClearState();
}