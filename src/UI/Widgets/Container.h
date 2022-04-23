#pragma once

#include <UI/UI.h>

namespace UI
{
   
struct Container : public Node {
    Container(){}
    void render(){
        Renderer::DrawQuad(position, size,{0,0,0,1});
    }
};

}