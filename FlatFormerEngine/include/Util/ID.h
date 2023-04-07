#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Util {

	template
		<typename id_value_type,
		size_t index_bits,			// number of bits per id
		size_t version_bits>		// number of bits per version
	union ffe_id
	{
		using value_type = id_value_type;

		static_assert(sizeof(value_type) * 8 >= (index_bits + version_bits), "ASSERT: Invalid ID layout. More bits used than id_value_type can hold!");

		static constexpr size_t			INDEX_BIT_COUNT		{ index_bits };
		static constexpr size_t			VERSION_BIT_COUNT	{ version_bits };

		static constexpr value_type		MIN_VERISON			{ 0 };
		static constexpr value_type		MAX_VERSION			{ (1U << VERSION_BIT_COUNT) - 2U };
		static constexpr value_type		MAX_INDICES			{ (1U << INDEX_BIT_COUNT) - 2U };

		static constexpr value_type		INVALID_ID			{ std::numeric_limits<value_type>::max() };

	private:
		value_type	value;

	public:
		struct
		{
			value_type					index : INDEX_BIT_COUNT;		// number of bits per indexes
			value_type					version : VERSION_BIT_COUNT;	// number of bits per version
		};

		ffe_id() :
			index(0),
			version(0)
		{}

		ffe_id(value_type value) :
			value(value)
		{}

		ffe_id(value_type index, value_type version) :
			index(index),
			version(version)
		{}

		inline operator value_type() const
		{
			return value;
		}

		inline bool IsNull() const
		{
			return value == 0;
		}
	};



	/// Defines 32-bit id
	/// Max. possible id								: 1048576
	/// Max. possible versions until loop per handle	: 4096
	using ffe_id32 = ffe_id<uint32, 20, 12>;			// 20 bits per id, 12 bits per version

	/// Defines 64-bit id
	/// Max. possible id								: 1099511627776
	/// Max. possible versions until loop per handle	: 16777216
#ifdef ECS_64BIT
	using ffe_id64 = ffe_id<uint64, 40, 24>;			// 40 bits per id, 24 bits per version
#else
	using ffe_id64 = ffe_id32;
#endif

	using ID = ffe_id64;

}}