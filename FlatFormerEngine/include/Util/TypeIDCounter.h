#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Util {

		// TypeIDCounter - type counter used to optimize search in manager structures
		template<class ClassUser>
		class TypeIDCounter
		{
		private:
			static TypeID s_Count;    // type counter

		public:
			// Returns the TypeID for the given template subtype
			template<class ClassType>
			static const TypeID Get()
			{
				static const TypeID STATIC_TYPE_ID{ s_Count++ };
				return STATIC_TYPE_ID;
			}


			// Returns the number of subtypes for the class
			static const TypeID GetCount()
			{
				return s_Count;
			}
		};

}}