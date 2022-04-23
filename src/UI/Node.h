#pragma once

#include <Rendering/Renderer.h>
#include <Assets/Assets.h>
#include <algorithm>

namespace UI {

struct Node {
    Node* parent = nullptr;
    Node* children = nullptr;
    Node* next = nullptr;

    glm::vec2 position = {};

    glm::vec2 offset = {};
    glm::vec2 size = {};

    virtual void update(int dt){};
    virtual void render(){};
    
    virtual void renderClipped(){};
    bool clippedContents = false;

    virtual void stencil(){};
    bool hasStencil = false;

    // Callbacks
    std::function<void(int,int)> clickCallback;
    std::function<void(const SDL_KeyboardEvent&)> keyCallback;
    std::function<void(const SDL_TextInputEvent&)> textCallback;
    std::function<void(const SDL_MouseMotionEvent&)> dragCallback;
    virtual void loseFocus(){};

    virtual ~Node() = default;

    // Lifetime management
    void addChild(Node* toAdd);
    void deparent(Node* toDetach);
    void detatch();
    void destroy();
    void destroy_recursive();

    // Utility
    Node* const last();

    Node* previousChild();
    Node* nextChild();

    inline bool hitTest(float hitX, float hitY){
        return (hitX >= position.x && hitX <= position.x + size.x && hitY >= position.y && hitY <= position.y + size.y);
    }

    // Positioning
    void MoveTo(const glm::vec2& newPosition);

    // Layout
    void ApplyOffsets();
    void listLayout();
    void barLayout();

    void gridLayout(int columns);

    void center();
    void right();

};

}