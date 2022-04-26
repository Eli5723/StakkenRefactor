#include <Application.h>

#include <Rendering/Renderer.h>
#include <Rendering/ScreenQuad.h>
#include <Assets/Assets.h>
#include <Assets/Sounds.h>
#include <UI/UI.h>

#include <Input/InputProfile.h>
#include <Data/Identity.h>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <types.h>
#include <thread>

#include <States/MenuState.h>

#include <Input/Input.h>
#include <Game/Demo.h>

#include <filesystem>
#include <location.h>

#include <Networking/Client.h>

// #include <LocklessQueue.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

Application* Application::instance;
DemoPlayer* test;

int Application::init() {
    instance = this;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    gl3wInit();

    window = SDL_CreateWindow("Stakken", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    target_fps(1000);

    resolution = { WINDOW_WIDTH, WINDOW_HEIGHT };
    Renderer::Init(resolution);
    ScreenQuad::Init();

    // Set the working Directory to the executable location before attempting to load assets 
    std::filesystem::current_path(Location::ExecutablePath());

    Assets::Init();
    Assets::active_shader = Assets::bgShaders.get("./resources/shaders/Ocean.frag");
    Assets::active_font = Assets::fonts.get("./resources/fonts/Lato-Regular.ttf");
    Assets::active_texture = Assets::textures.get("./resources/pieceTextures/TGF2.png");
    Sounds::loadDirectory("./resources/sounds");

    local_controller = Input::CreateKeyboard();
    local_identity = Identity::LoadRandom();

    UI::Init(resolution);

    Demo* demo = Demo::Load("./demos/last_played.rep");
    test = new DemoPlayer;
    test->demo = demo;
    test->restart();

    // auto a = new UI::GameViewer;
    // a->game = &test->target;
    // a->owner = local_identity;

    // UI::AddToScreen(a);

    // network_client.Connect("127.0.0.1", 60000);

    state_set(new MenuState());
    return 0;
}

int Application::execute() {
    u32 frameBegin = SDL_GetTicks();
    u32 frameDelta = 0;

    u64 start;
    u64 end;

    SDL_Event event;
    running = true;
    while (running) {
        start = SDL_GetPerformanceCounter();

        int newTime = SDL_GetTicks();
        frameDelta = newTime - frameBegin;
        frameBegin = newTime;

        while (SDL_PollEvent(&event)) {
            events(event);
        }

        Input::Update(frameDelta);
        update(frameDelta, newTime);
        render(frameDelta, newTime);
        Input::Clear();

        end = SDL_GetPerformanceCounter();
        float elapsedMicroseconds = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000000.0f;

        // Cap FPS  
        if (elapsedMicroseconds < frame_delay) {
            std::this_thread::sleep_for(std::chrono::microseconds((u64)(frame_delay - elapsedMicroseconds)));
        }
    }

    return 0;
}

void Application::update(int dt, int time) {
    state_stack.back()->update(dt, time);

    UI::Update(dt);

    // test->advance(dt);
}

void Application::render(int dt, int time) {
    float fTime = (float)time / 1000.0f;

    glClearColor(0, 0, 0, 1);
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (Assets::active_shader)
        Assets::active_shader->draw(&resolution.x, fTime);

    Renderer::BeginBatch();
    state_stack.back()->render(dt, time);
    Renderer::EndBatch();
    Renderer::Flush();

    // Render UI
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    Renderer::BeginBatch();
    UI::RenderStencil();
    Renderer::EndBatch();
    Renderer::Flush();

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    Renderer::BeginBatch();
    UI::RenderClipped();
    Renderer::EndBatch();
    Renderer::Flush();

    glStencilFunc(GL_ALWAYS, 99, 0xFF);
    glStencilMask(0xFF);
    Renderer::BeginBatch();
    UI::Render();
    Renderer::EndBatch();
    Renderer::Flush();

    // Post Frame
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    SDL_GL_SwapWindow(window);
}

void Application::events(const SDL_Event& event) {
    switch (event.type) {
    case SDL_QUIT:
        running = false;
        break;

    case SDL_WINDOWEVENT: {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            on_resize(event.window.data1, event.window.data2);
            break;
        }
    } break;

    case SDL_MOUSEBUTTONDOWN:
        // TODO: ADD UI INPUT SCALING. ADD STATE TO UI OR NO?
        UI::Click((event.button.x - game_offset.x), (event.button.y - game_offset.y));
        break;

    case SDL_MOUSEBUTTONUP:
        UI::EndDrag();
        break;

    case SDL_MOUSEMOTION:
        UI::MoveCapture(event.motion);
        break;

    case SDL_KEYDOWN: {
        // if (ClientKeyEvent(event.key))
        //     return;      


        if (state_stack.back()->key_capture(event.key))
            return;

        // TODO: Add hover control
        if (UI::KeyCapture(event.key))
            return;

        if (!event.key.repeat)
            Input::KeyEvent(event.key);


        if (event.key.keysym.sym == SDLK_F5)
            Assets::load_next_shader();

        if (event.key.keysym.sym == SDLK_F6)
            Assets::load_next_texture();

        if (event.key.keysym.sym == SDLK_F7)
            Sounds::Toggle();
    }

    case SDL_TEXTINPUT:
        UI::InputCapture(event.text);
        break;

    }
}

void Application::on_resize(int width, int height) {
    resolution.x = width;
    resolution.y = height;
    glViewport(0, 0, width, height);

    // Calculate largest scale that can fit within the window
    glm::vec2 scaleVec = resolution / game_resolution;
    game_scale = std::min(scaleVec.x, scaleVec.y);

    // Calculate offset required to center game
    glm::vec2 effectiveSize = game_resolution * game_scale;
    game_offset = (resolution / 2.0f) - (effectiveSize / 2.0f);

    glm::mat4 viewTransform = glm::mat4(1.0f);
    viewTransform = glm::scale(viewTransform, glm::vec3{ game_scale, game_scale, 0 });
    viewTransform = glm::translate(viewTransform, glm::vec3{ game_offset.x / game_scale, game_offset.y / game_scale, 0 });
    Renderer::UpdateTransform(viewTransform, 0);

    glm::mat4 viewProjection = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f);
    Renderer::UpdateProjection(viewProjection);
}

void Application::target_fps(int target) {
    frame_delay = (1.0f / (float)target) * 1000000.0f;
}

void Application::state_set(State* newState) {
    if (state_stack.size() > 0) {
        state_stack.back()->close();
        state_stack.pop_back();
    }
    newState->init();
    state_stack.push_back(newState);
}
void Application::state_push(State* newState) {
    if (state_stack.size() > 0) {
        state_stack.back()->close();
        state_stack.pop_back();
    }
    newState->init();
    state_stack.push_back(newState);
}
void Application::state_pop() {
    if (state_stack.size() > 0) {
        state_stack.back()->close();
        state_stack.pop_back();
    }
}
