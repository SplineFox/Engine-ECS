#pragma once

#include "Engine/API.h"
#include "ECS/IComponent.h"

namespace FlatFormer { namespace ECS {

	class ComponentFilter
	{
	private:
		std::bitset<32> m_Bitset;

	public:

		template<class ComponentType>
		void SetItemByType(bool value)
		{
			ComponentTypeID CTID = ComponentType::STATIC_TYPE_ID;
			m_Bitset[CTID] = value;
		}


		void SetItemByIndex(unsigned int index, bool value)
		{
			if (index < m_Bitset.size())
			{
				m_Bitset[index] = value;
			}
		}


		bool IsContain(const ComponentFilter& filter)
		{
			return (m_Bitset & filter.m_Bitset) == filter.m_Bitset;
		}
	};

}}
