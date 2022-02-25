#pragma once

#include <functional>

#include <glm/glm.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

#include <UI/Node.h>
#include <UI/NodeTypes.h>

namespace UI {

void Init(const glm::vec2& resolution);
void Resize(const glm::vec2& resolution);
void AddToScreen(Node* element);

const glm::vec2& Resolution();

// Keyboard Input
bool KeyCapture(const SDL_KeyboardEvent& event);
bool InputCapture(const SDL_TextInputEvent& event);
void SetFocus(Node* toFocus);
void ClearFocus();

// Mouse Based Input
void MoveCapture(const SDL_MouseMotionEvent& event);
void SetIgnoreMovement();
void EndDrag();
void Click(int x, int y);

// Keyboard Based Dragging
void CalculatePositions();

void Update(int dt);

void Render(Node* root, glm::vec2& offset);
void Render();

void RenderClipped(Node* root, glm::vec2& offset);
void RenderClipped();

void RenderStencil();

void SetHover(Node* target);
void SetHoverSmooth(Node* target);


void HoverNext();
void HoverPrevious();
void ClearHover();
void SelectHover();

void ClearState();


}