#pragma once

#include "Engine/API.h"
#include "Render/RenderStructs.h"

namespace FlatFormer {

	static constexpr ColorRGBA CLEAR_COLOR					{ 85,  85,  85,  255 };
	static constexpr ColorRGBA GRID_COLOR					{ 71,  71,  71,  255 };

	static constexpr ColorRGBA WHITE_COLOR					{ 255,  255,  255,  255 };
	static constexpr ColorRGBA GREY_COLOR					{ 127,	127,  127,	255 };
	static constexpr ColorRGBA BLACK_COLOR					{ 0,	0,	  0,	255 };

	static constexpr ColorRGBA RED_COLOR					{ 255,  0,    0,    255 };
	static constexpr ColorRGBA GREEN_COLOR					{ 0,    255,  0,    255 };
	static constexpr ColorRGBA BLUE_COLOR					{ 0,    0,    255,  255 };

	static constexpr ColorRGBA DARK_RED_COLOR				{ 155,  0,    0,    255 };
	static constexpr ColorRGBA DARK_GREEN_COLOR				{ 0,    155,  0,    255 };
	static constexpr ColorRGBA DARK_BLUE_COLOR				{ 0,    0,    155,  255 };

	static constexpr ColorRGBA CYAN_COLOR					{ 0,	255,  255,  255 };
	static constexpr ColorRGBA MAGENTA_COLOR				{ 255,  0,    255,  255 };
	static constexpr ColorRGBA YELLOW_COLOR					{ 255,  255,  0,    255 };

	static constexpr ColorRGBA LIGHT_GREEN_COLOR			{ 128,  255,  138,    255 };
	static constexpr ColorRGBA LIGHT_BLUE_COLOR				{ 0,  190,  255,    255 };
	static constexpr ColorRGBA LIGHT_GREEN_BLUE_COLOR		{ 3,  223,  165,    255 };
	static constexpr ColorRGBA LIGHT_PINK_COLOR				{ 250,  120,  120,    255 };
	static constexpr ColorRGBA LIGHT_VIOLET_COLOR			{ 160,  120,  250,    255 };

	static constexpr ColorRGBA ORANGE_COLOR					{ 255,  100,  0,    255 };


	static constexpr ColorRGBA COLLIDER_COLOR				{ LIGHT_GREEN_COLOR };
	static constexpr ColorRGBA RIGIDBODY_COLOR				{ CYAN_COLOR };
	static constexpr ColorRGBA SPRITE_COLOR					{ GREY_COLOR };

}