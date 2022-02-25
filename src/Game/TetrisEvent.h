#pragma once

#include <types.h>

enum class TetrisEvent : u8 {
    None,
    
    // Inputs
    Left,
    Right,
    SoftDrop,
    HardDrop,
    SonicDrop,
    RCW,
    RCCW,
    Flip,

    // Gamemode Events
    ClientSafeEvent,
    Gravity,
    Creep,
    RemoveRow,
    AddRow,
    SendLines,


    // Utility
    Count
};