#pragma once

#include <UI/UI.h>
#include <UI/Node.h>

#include <Assets/Assets.h>
#include <Rendering/Renderer.h>

#include <HSV.h>

#include <Rendering/RenderGame.h>
#include <Data/Identity.h>

#include <Assets/Sounds.h>
#include <Input/Input.h>

#include <Game/DemoPlayer.h>

#include <UI/Widgets/NumericInput.h>
#include <UI/Widgets/HiddenInput.h>
#include <UI/Widgets/Input.h>
#include <UI/Widgets/Checkbox.h>
#include <UI/Widgets/PieceEditor.h>
#include <UI/Widgets/ColorRotationEditor.h>
#include <UI/Widgets/Slider.h>
#include <UI/Widgets/Label.h>
#include <UI/Widgets/Image.h>
#include <UI/Widgets/ImageBacked.h>
#include <UI/Widgets/DraggableIcon.h>
#include <UI/Widgets/Button.h>
#include <UI/Widgets/TextButton.h>
#include <UI/Widgets/GameViewer.h>
#include <UI/Widgets/Container.h>


namespace UI {

const std::string icons[] = {
    "left.png",
    "right.png",
    "softdrop.png",
    "harddrop.png",
    "sonicdrop.png",
    "rcw.png",
    "rccw.png",
    "flip.png",
    "das.png",
    "arr.png",
    "droparr.png"
};


}