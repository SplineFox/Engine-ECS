#pragma once

#include "Engine/Platform.h"
#include "Log/Logger.h"

#define ENITY_MAX_NUM_SIZE					512

#define COMPONENT_MAX_NUM_SIZE				512

#define ID_POOL_CAPACITY					1024

#define EVENT_MEMORY_BUFFER_SIZE			12800

static constexpr size_t				GROW_ELEMENT_SIZE		= 128;

static constexpr float				FFE_MAX_FLOAT			= FLT_MAX;

static constexpr float				FFE_PI					= 3.14159265359;


#pragma region Using
	using uint8 = FlatFormer::uint8;
	using float32 = FlatFormer::float32;
#pragma endregion

//===========================CollisionSystem============================

static constexpr uint8		FFE_MAX_VERTICES_COUNT				= 8;		// number of polygon vertices

static constexpr float32	FFE_PENITRATION_DEPTH_CORRECTION	= 0.9;		// adjusting the penetration depth of colliders

static constexpr int		FFE_MAX_TREE_MEMORY_CAPACITY		= 128000;	// amount of memory in bits for all trees (16 kb)

static constexpr int		FFE_MAX_TREE_DEEP					= 8;		// height of the cell of the dividing grid by default

static constexpr int		FFE_MAX_TREE_COUNT					= 1000;		// maximum number of trees

static constexpr int		FFE_MIN_TREE_NODE_WIDTH				= 1;		// minimum width of the tree node

static constexpr int		FFE_MIN_TREE_NODE_HEIGHT			= 1;		// minimum height of the tree node

static constexpr int		FFE_MAX_TREE_NODE_CAPACITY			= 4;		// maximum node capacity

static constexpr int		FFE_SEPARATION_CELL_WIDTH_DEFAULT	= 500;		// default grid cell width

static constexpr int		FFE_SEPARATION_CELL_HEIGHT_DEFAULT	= 500;		// height of the cell of the dividing grid by default


//===========================PhysicsSystem==============================

static constexpr float		FFE_DEFAULT_DENSITY					= 0.5f;

static constexpr float		FFE_DEFAULT_STRENGTH				= 0.5f;

static constexpr float		FFE_DEFAULT_RESTITUTION				= 0.9f;

static constexpr float		FFE_DEFAULT_STATIC_FRICTION			= 0.6f;

static constexpr float		FFE_DEFAULT_DYNAMIC_FRICTION		= 0.4f;

static constexpr float		FFE_GRAVITY							= -9.8f;

namespace FlatFormer {

	

}