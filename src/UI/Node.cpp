#include <UI/Node.h>

namespace UI {

// Lifetime management
void Node::addChild(Node* toAdd){
    toAdd->parent = this;

    if (children == nullptr){
        children = toAdd;
    } else {
        last()->next = toAdd;
    }
}

void Node::deparent(Node* child){
    Node* neighbor = parent->children;
    
    // Case: Child is first in list
    if (parent->children == child){
        parent->children = child->next;
        child->next = nullptr;
        return;
    }
    
    // Case: Child is not first in list
    for (Node* neighbor = parent->children; neighbor; neighbor = neighbor->next){
        if (neighbor->next != child)
            continue;

        neighbor->next = child->next;
        child->next = nullptr;
    }
}

void Node::detatch(){
    deparent(this);
    parent = nullptr;
}

void Node::destroy(){
    deparent(this);
    delete this;
}

void Node::destroy_recursive(){
    Node* next;
    for (Node* child = children; child;){
        next = child->next;
        child->destroy_recursive();
        child = next;
    }

    destroy();
}

void Node::destroy_children(){
    Node* next;
    for (Node* child = children; child;){
        next = child->next;
        child->destroy_recursive();
        child = next;
    }

    children = nullptr;
}

// Utility
Node* const Node::last(){
    Node* child = children;

    while (child->next != nullptr){
        child = child->next;
    }

    return child;
}

Node* Node::previousChild(){
    for (Node* child = parent->children; child; child = child->next){
        if (child->next == this)
            return child;
    }

    return parent->last();
}

Node* Node::nextChild(){
    if (next)
        return next;

    return parent->children;
}

// Positioning
void Node::MoveTo(const glm::vec2& newPosition){
    position = newPosition;
    ApplyOffsets();
}

// Layout
void Node::ApplyOffsets(){
    Node* child = children;
    while (child != nullptr){
        child->position = position + child->offset;

        child->ApplyOffsets();
        child = child->next;
    }
}

void Node::listLayout(){
    Node* child = children;
    float height = 0;
    float width = 0;

    while (child != nullptr){
        child->offset.y = height;
        height += child->size.y;
        width = std::max(child->size.x, width);

        child = child->next;
    }

    size = {width,height};
    ApplyOffsets();
}

void Node::barLayout(){
    Node* child = children;

    size.x = 0;
    size.y = 0;

    while (child != nullptr){
        child->offset.x = size.x;
        size.x += child->size.x;
        size.y = std::max(child->size.y, size.y);

        child = child->next;
    }

    ApplyOffsets();
}

void Node::gridLayout(int columns){
    Node* child = children;
    float height = 0;
    float width = 0;

    float cx = 0;
    float rowMaxHeight = 0;

    int i = 0;
    while (child != nullptr){
        child->offset.y = height;
        rowMaxHeight = std::max(child->size.y, rowMaxHeight);

        child->offset.x = cx;
        cx += child->size.x;
        width = std::max(cx, width);
        
        i++;
        if (i%columns == 0) {
            height += rowMaxHeight;
            rowMaxHeight = 0;
            cx = 0;
        }

        child = child->next;
    }

    size = {width,height + rowMaxHeight};
    ApplyOffsets();
}

void Node::center(){
    position = parent->size/2.0f - size/2.0f;
    ApplyOffsets();
}

void Node::right(){
    position.x = parent->size.x - size.x;
    position.y = parent->position.y;
    ApplyOffsets();
}


}