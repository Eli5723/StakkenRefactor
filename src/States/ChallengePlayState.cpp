#include <States/ChallengePlayState.h>

#include <Application.h>
#include <UI/UI.h>

#include <States/ChallengeSelectState.h>

#include <Game/Game.h>
#include <Challenges/Challenges.h>

UI::Node* exitButton;

UI::Node* challengeGameViewer;

UI::Node* challengeTitle;
UI::Node* challengeDescription;

Game* board;

void ChallengePlayState::init(){
    // Exit Button
    exitButton = new UI::TextButton("Go Back", [](int,int){
        Application::instance->state_set(new ChallengeSelectState);
    });
    UI::AddToScreen(exitButton);

    // Game
    board = new Game;
    board->rules = Challenges::getChallenge(Application::instance->challenge_mode);
    board->Reset(clock());

    challengeGameViewer = new UI::GameViewer(board, Application::instance->local_identity);
	challengeGameViewer->position = {RenderGame::PADDING, UI::Resolution().y - RenderGame::PLAYER_DIMENSIONS.y - RenderGame::PADDING};
    UI::AddToScreen(challengeGameViewer);

    // Game Objective
    challengeTitle = new UI::Label(Challenges::getChallengeName(Application::instance->challenge_mode));
    UI::AddToScreen(challengeTitle);
    challengeDescription = new UI::Label(Challenges::getChallengeDescription(Application::instance->challenge_mode));
    UI::AddToScreen(challengeDescription);
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

    return false;
}

void ChallengePlayState::close(){
    exitButton->destroy_recursive();
    challengeGameViewer->destroy();
    delete board;

    challengeTitle->destroy();
    challengeDescription->destroy();

    UI::ClearState();
}