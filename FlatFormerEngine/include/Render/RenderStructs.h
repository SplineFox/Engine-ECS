#pragma once

#include "Engine/API.h"

namespace FlatFormer {

	struct ColorRGBA
	{
		uint8 r = 255;
		uint8 g = 255;
		uint8 b = 255;
		uint8 a = 255;
	};


	enum FlipType
	{
		None		= 0x00000000,
		Horizontal	= 0x00000001,
		Vertical	= 0x00000002
	};


	struct Point
	{
		int x = 0;
		int y = 0;
	};


	struct Rect
	{
		int x = 0, y = 0;
		int w = 0, h = 0;
	};

}