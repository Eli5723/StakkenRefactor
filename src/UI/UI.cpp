#include <UI/UI.h>

#include <glm/geometric.hpp>
#include <cmath>

namespace UI {

struct Data {
    Node* root;

    Node* focus = 0;
    Node* hover = 0;

    Node* drag = 0;
    glm::vec2 dragStartingPosition;
    bool ignoreMovement = false;
} data;

struct HoverBox {
    glm::vec2 lastPosition = {0, 0};
    glm::vec2 targetPosition = {500, 0};

    glm::vec2 lastSize = {0, 0};
    glm::vec2 targetSize = {0, 0};

    glm::vec2 pos = {0, 0};
    glm::vec2 size = {0, 0};

    float lerp = 1;

    void set(Node* last, Node* target){
        if (last) {
            lastPosition = last->position;
            lastSize = last->size;
        } else {
            lastPosition = target->position;
            lastSize = target->size;
        }

        targetPosition = target->position;
        targetSize = target->size;
        lerp = 1;
    }

    void setSmooth(Node* last, Node* target){
        if (last) {
            lastPosition = pos;
            lastSize = size;
        } else {
            lastPosition = target->position;
            lastSize = target->size;
        }

        targetPosition = target->position;
        targetSize = target->size;
        lerp = 1;
    }


    void update(int dt){
        lerp -= (dt/250.0f);
        lerp = fmax(lerp, 0);

        glm::vec2 dist = targetPosition - lastPosition;
        glm::vec2 sizeDif = targetSize - lastSize;


        float f = (1.0f-lerp);
        f*=f*f;
        pos = lastPosition + dist * f;
        size = lastSize + sizeDif * f;
    }

    void draw() {
        Renderer::QuadBox(pos, size, 1.0f, {1.0f,1.0f,1.0f,1.0f});
    }
} hoverBox;

void Init(const glm::vec2& resolution){
    data.root = new Node;
    data.root->size = resolution;
    ClearState();
}

void Resize(const glm::vec2& resolution){
    data.root->size = resolution;
}

const glm::vec2& Resolution(){
    return data.root->size;
}

void AddToScreen(Node* node){
    data.root->addChild(node);
}

bool KeyCapture(const SDL_KeyboardEvent& event){
    if (data.focus && data.focus->keyCallback) {
        data.focus->keyCallback(event);
        return true;
    }

    // TODO: Reimplement when I figure out a way to manage gameplay focus
    // if (event.keysym.sym == SDLK_UP  || event.keysym.sym == SDLK_LEFT){
    //     UI::HoverPrevious();
    //     return true;
    // }

    // if (event.keysym.sym == SDLK_DOWN || event.keysym.sym == SDLK_RIGHT){
    //     UI::HoverNext();
    //     return false;
    // }

    // if (event.keysym.sym == SDLK_SPACE){
    //     UI::SelectHover();
    //     return false;
    //}

    return false;
}

bool InputCapture(const SDL_TextInputEvent& event){
    if (data.focus && data.focus->textCallback) {
        data.focus->textCallback(event);
        return true;
    }
    return false;
}

void SetFocus(Node* toFocus){
    data.focus = toFocus;
}

void ClearFocus(){
    data.focus = 0;
    SDL_StopTextInput();
}

void MoveCapture(const SDL_MouseMotionEvent& event){
        if (data.ignoreMovement){
            data.ignoreMovement = false;
            return;
        }

        if (data.drag && data.drag->dragCallback) {
            data.drag->dragCallback(event);
            return;
        }

        // Match hover with mouse location
        if (data.hover){
            for (Node* neighbor = data.hover->parent->children; neighbor; neighbor = neighbor->next){
                if (neighbor == data.hover)
                    continue;

                if (neighbor->hitTest(event.x, event.y)){
                    SetHoverSmooth(neighbor);
                    break;
                }
            }
        }
}
void SetIgnoreMovement(){
    data.ignoreMovement = true;
}

void EndDrag(){
    if (data.drag){
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_WarpMouseInWindow(0, data.drag->position.x + data.dragStartingPosition.x, data.drag->position.y + data.dragStartingPosition.y);
        }

    data.drag = 0;
}

void CalculatePositions(){
    data.root->ApplyOffsets();
}

void Render(Node* root, const glm::vec2& cursor){
    root->render();

    for (Node* child = root->children; child; child = child->next)
        Render(child, cursor + root->offset);

}

void Render(){
    Render(data.root, {0,0});

    if (data.hover)
        hoverBox.draw();
}

void RenderClipped(Node* root, const glm::vec2& cursor){
    if (root->clippedContents)
        root->renderClipped();

    Node* current = root->children;
    while (current != NULL){
        RenderClipped(current, cursor + root->offset);

        current = current->next;
    }
}

void RenderClipped(){
    RenderClipped(data.root, {0,0});
}

void RenderStencil(Node* root, const glm::vec2& cursor){
    if (root->hasStencil)
        root->stencil();

    Node* current = root->children;
    while (current != NULL){
        RenderStencil(current, cursor + root->offset);

        current = current->next;
    }
}

void RenderStencil(){
    RenderStencil(data.root, {0,0});
}

bool Click(Node* root, int x, int y){
    if (!root->hitTest(x,y))
        return false;

    if (root->clickCallback) {
        root->clickCallback(x - root->position.x, y - root->position.y);
        Sounds::play(Sounds::Slot::Click);
        return true;
    } else if (root->dragCallback){
        // Used to hide the cursor while dragging so that the user can focus
        SDL_SetRelativeMouseMode(SDL_TRUE);
        data.drag = root;
        data.dragStartingPosition = {x -root->position.x, y - root->position.y};
        return true;
    }

    for (Node* child = root->children; child; child = child->next){
        if (Click(child, x, y))
                    return true;
    }


    return false;
}

void Click(int x, int y){
    Click(data.root, x, y);
}

void Update(Node* root, int dt){
    root->update(dt);

    Node* current = root->children;
    while (current != NULL){
        Update(current, dt);

        current = current->next;
    }
}

void Update(int dt){
    Update(data.root, dt);

    if (data.hover)
        hoverBox.update(dt);
}

void RenderHover(){

}

void SetHover(Node* target){
    if (target == data.hover)
        return;

    hoverBox.set(data.hover, target);
    data.hover = target;
}

void SetHoverSmooth(Node* target){
    if (target == data.hover)
        return;

    hoverBox.setSmooth(data.hover, target);
    data.hover = target;
}

void HoverPrevious(){
    if (!data.hover)
        return;

    Node* prev = data.hover->previousChild();
    SetHover(prev);
}

void HoverNext(){
    if (!data.hover)
        return;

    Node* next = data.hover->nextChild();
    SetHover(next);
}

void ClearHover(){
    data.hover = 0;
}

void SelectHover(){
    if (data.hover && data.hover->clickCallback)
        data.hover->clickCallback(0,0);
}

void ClearState(){
    ClearHover();
    ClearFocus();
    EndDrag();
}

}