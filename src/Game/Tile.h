#pragma once

#include <types.h>

using Tile = s8;

enum TileType : Tile {
    Empty = -1,
    T,
	O,
	L,
	J,
	Z,
	S,
	I,
	Garbage,
	Invalid
};

using Connection = u8;
enum Side : Connection {
    Up    = 0b1000,
    Right = 0b0100,
    Down  = 0b0010,
    Left  = 0b0001,
};