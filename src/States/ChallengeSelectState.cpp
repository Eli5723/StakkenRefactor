#include <States/ChallengeSelectState.h>

#include <Application.h>
#include <UI/UI.h>
#include <States/ChallengePlayState.h>
#include <States/MenuState.h>
#include <Challenges/Challenges.h>


UI::Node* challengeList;

void ChallengeSelectState::init(){
    challengeList = new UI::Node;

    UI::Node* challengeSelectItem;

    challengeSelectItem = new UI::TextButton("Sprint", [](int,int){
        Application::instance->challenge_mode = Challenges::SPRINT;
        Application::instance->state_set(new ChallengePlayState(Challenges::CHALLENGE::SPRINT));
    });
    challengeList->addChild(challengeSelectItem);

    challengeSelectItem = new UI::TextButton("Survival", [](int,int){
        Application::instance->challenge_mode = Challenges::SURVIVAL;
        Application::instance->state_set(new ChallengePlayState(Challenges::CHALLENGE::SURVIVAL));
    });
    challengeList->addChild(challengeSelectItem);

    challengeSelectItem = new UI::TextButton("z2sam", [](int,int){
        Application::instance->challenge_mode = Challenges::Z2SAM;
        Application::instance->state_set(new ChallengePlayState(Challenges::CHALLENGE::Z2SAM));
    });
    challengeList->addChild(challengeSelectItem);

    challengeSelectItem = new UI::TextButton("Back", [](int,int){
        Application::instance->state_set(new MenuState);
    });
    challengeList->addChild(challengeSelectItem);

    challengeList->barLayout();
    UI::AddToScreen(challengeList);
    challengeList->center();

    UI::SetHover(challengeSelectItem);

}

void ChallengeSelectState::update(int dt, int time){

}

void ChallengeSelectState::render(int dt, int time){

}

void ChallengeSelectState::close(){
    challengeList->destroy_recursive();
    UI::ClearState();
}