#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Memory {

	class IObjectDispatcher
	{
	public:
		virtual ~IObjectDispatcher()
		{}

		// Allocates memory for an object
		virtual void *			Place() = 0;
		// Frees the memory occupied by an object
		virtual void			Replace(void *) = 0;
		// Returns the number of objects in the list
		virtual inline size_t	GetObjectCount() const = 0;
	};
}}