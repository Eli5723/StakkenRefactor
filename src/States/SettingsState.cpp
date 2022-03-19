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

Identity* testIdentity;

void SettingsState::init(){
    testBoard = new Game;

	settings_bar = new UI::Node();
	settings_bar->addChild(new UI::TextButton(glm::vec2{192,48}, "Back", [](int, int){
		Application::instance->state_set(new MenuState());
	}));
	settings_bar->addChild(new UI::TextButton(glm::vec2{192,48}, "Shader (F5)", [](int, int){
		Assets::BGShader* shader = Assets::bgShaders.get(Assets::shaderDirectory.next());
		if (shader)
			Assets::active_shader = shader;
	}));
	settings_bar->addChild(new UI::TextButton(glm::vec2{192,48}, "Texture (F6)", [](int, int){
		Assets::Texture* texture = Assets::textures.get(Assets::textureDirectory.next());
		if (texture)
			Assets::active_texture = texture;
	}));
	settings_bar->addChild(new UI::Checkbox(glm::vec2{192,48}, Sounds::enabled, "Sound (F7)"));
	settings_bar->addChild(new UI::Slider<float>(glm::vec2{192,48}, 0.0f, 1.0f, RenderGame::settings.backgroundOpacity, "BG Alpha"));
	settings_bar->listLayout();

	UI::AddToScreen(settings_bar);
	settings_bar->right();
	settings_bar->position.x -= RenderGame::PADDING;
	settings_bar->position.y += RenderGame::PADDING;
	settings_bar->ApplyOffsets();

	testIdentity = Identity::LoadRandom();

	identity_editor = new UI::ColorRotationEditor(*testIdentity);
	UI::AddToScreen(identity_editor);
	identity_editor->position.x = settings_bar->position.x - identity_editor->size.x;
	identity_editor->position.y = settings_bar->position.y;
	identity_editor->ApplyOffsets();

	gameViewer = new UI::GameViewer(testBoard, testIdentity);
	gameViewer->position = {RenderGame::PADDING, UI::Resolution().y - RenderGame::PLAYER_DIMENSIONS.y - RenderGame::PADDING};

	UI::AddToScreen(gameViewer);

	UI::SetHover(settings_bar->children);
}

void SettingsState::update(int dt, int time){

}

void SettingsState::render(int dt, int time){
    // float fTime = (float)time/1000.0f;
}

void SettingsState::close(){
	settings_bar->destroy_recursive();
	identity_editor->destroy_recursive();
	gameViewer->destroy();

	UI::ClearState();

	delete testIdentity;
    delete testBoard;

}