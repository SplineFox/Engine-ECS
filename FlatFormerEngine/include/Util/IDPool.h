#pragma once

#include "Util/ID.h"

namespace FlatFormer { namespace Util {

	class IDPool
	{
	private:
		// Next avaliable ID
		// (if there are no free IDs in m_FreeList)

		typename ID::value_type m_NextID;

		// IDs that are avaliable to reuse
		std::vector<ID> m_FreeList;

		// Support for actual versions of identifiers index is the element number in the array, and version is the value
		std::vector<typename ID::value_type> m_Versions;


		IDPool(const IDPool&) = delete;
		IDPool(IDPool&&) = delete;
		IDPool& operator=(const IDPool&) = delete;
		IDPool& operator=(IDPool&&) = delete;


	public:
		explicit IDPool(int poolSize) :
			m_Versions(poolSize),
			m_NextID(0)
		{}

		ID AcquireID()
		{
			ID id;

			// if there is an id in the free id list, which can be reused
			if (!m_FreeList.empty())
			{
				id = m_FreeList.back();
				m_FreeList.pop_back();
			}
			else
			{
				id.index = m_NextID++;
				// created id must have version 1 because version 0 is the "invalid" version,
				// so you need to update the version in the pool
				m_Versions[id.index] = id.version = 1;
			}

			return id;
		}


		void ReleaseID(ID id)
		{
			/// after the id is freed its version needs to be changed
			/// as id with same index but old version should become invalid
			///
			/// example:
			/// ID id1 = idPool.Create();	<--- index = 1, version = 1
			/// idPool.Remove(id1);
			/// ID id2 = idPool.Create();	<--- index = 1, version = 2
			///
			/// id1 is now an invalid ID because the current version of identifiers with index = 1 is 2

			auto& newVersion = m_Versions[id.index];
			++newVersion;
			m_FreeList.emplace_back(static_cast<typename ID::value_type>(id.index), newVersion);
		}


		bool IsValid(ID id) const
		{
			if (id.index >= m_Versions.size())
				return false;

			return (id.version == m_Versions[id.index]) && (id.version > 0);
		}


		int GetSize() const
		{
			return m_NextID;
		}


		void Clear()
		{
			m_Versions.clear();
			m_FreeList.clear();
			m_NextID = 0;

	};

}}