#include "Util/TypeIDCounter.h"

namespace FlatFormer {

#pragma region Forward declaration
	namespace ECS
	{
		class IEntity;
		class IComponent;
		class ISystem;
	}
#pragma endregion

	namespace Util {

		template class TypeIDCounter<ECS::IComponent>;
		template class TypeIDCounter<ECS::ISystem>;

		TypeID TypeIDCounter<ECS::IComponent>::s_Count = 0u;
		TypeID TypeIDCounter<ECS::ISystem>::s_Count = 0u;

	}
}