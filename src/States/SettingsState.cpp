#include <States/SettingsState.h>

#include <UI/UI.h>
#include <States/MenuState.h>


#include <Rendering/RenderGame.h>
#include <Assets/Sounds.h>
#include <Game/Game.h>

#include <Data/Identity.h>

Game* testBoard; 

UI::Node* settings_bar;
UI::Node* settings_root;
UI::Node* identity_editor;
UI::Node* gameViewer;
UI::Node* keybinds;

Identity* testIdentity;

// Ruleset used to reset the board if the player fails
// TODO: Add the ability to send yourself garbage
struct TestGameRules : GameRules {
    void on_update(Game* game, int dt){};
    void on_place(Game* game,int cleared, int combo){};
    void on_win(Game* game){};
    void on_lose(Game* game){game->Reset(clock());};
    int score(Game* game){return 0;};
    bool result(Game* game){return 0;};
} rules;


void SettingsState::init(){
    testBoard = new Game;
	testBoard->Reset(clock());
	testBoard->rules = &rules;

	settings_bar = new UI::Node();
	settings_bar->addChild(new UI::TextButton(glm::vec2{192,48}, "Back", [](int, int){
		Application::instance->state_set(new MenuState());
	}));
	settings_bar->addChild(new UI::TextButton(glm::vec2{192,48}, "Shader (F5)", [](int, int){
		Assets::load_next_shader();
	}));
	settings_bar->addChild(new UI::TextButton(glm::vec2{192,48}, "Texture (F6)", [](int, int){
		Assets::load_next_texture();
	}));
	settings_bar->addChild(new UI::Checkbox(glm::vec2{192,48}, Sounds::enabled, "Sound (F7)"));
	settings_bar->addChild(new UI::Slider<float>(glm::vec2{192,48}, 0.0f, 1.0f, RenderGame::settings.backgroundOpacity, "BG Alpha"));
	settings_bar->listLayout();

	identity_editor = new UI::ColorRotationEditor(*Application::instance->local_identity);
	UI::AddToScreen(identity_editor);
	identity_editor->right();
	identity_editor->position.x -= RenderGame::PADDING;
	identity_editor->position.y += RenderGame::PADDING;
	identity_editor->ApplyOffsets();

	UI::AddToScreen(settings_bar);
	settings_bar->MoveTo({identity_editor->position.x + identity_editor->size.x - settings_bar->size.x, identity_editor->size.y + identity_editor->position.y});

	// identity_editor->position.x = settings_bar->position.x - identity_editor->size.x;
	// identity_editor->position.y = settings_bar->position.y;
	// identity_editor->ApplyOffsets();

	keybinds = Application::instance->local_controller->create_config_window();
	UI::AddToScreen(keybinds);
	keybinds->MoveTo({identity_editor->position.x - keybinds->size.x, identity_editor->position.y});
	keybinds->ApplyOffsets();

	gameViewer = new UI::GameViewer(testBoard, Application::instance->local_identity);
	gameViewer->position = {RenderGame::PADDING, UI::Resolution().y - RenderGame::PLAYER_DIMENSIONS.y - RenderGame::PADDING};

	UI::AddToScreen(gameViewer);
	UI::SetHover(settings_bar->children);
}

void SettingsState::update(int dt, int time){

	EventBuffer* events = Application::instance->local_controller->getEvents();
	testBoard->Events(events->events, events->count);

	testBoard->Update(dt);


}

void SettingsState::render(int dt, int time){
    // float fTime = (float)time/1000.0f;
}

bool SettingsState::key_capture(const SDL_KeyboardEvent& key){
	if (key.keysym.sym == SDLK_F2){
		testBoard->Reset(clock());
		return true;
	}


	return false;
}

void SettingsState::close(){
	settings_bar->destroy_recursive();
	identity_editor->destroy_recursive();
	gameViewer->destroy();

	keybinds->destroy_recursive();

	UI::ClearState();

	Application::instance->local_controller->save_config_window();

	delete testIdentity;
    delete testBoard;
}