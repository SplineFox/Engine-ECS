#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <cctype>
#include <algorithm>
#include <memory>


#include <vector>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <unordered_map>
#include <unordered_set>

#include <chrono>

#ifdef FFE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

namespace FlatFormer {

	using GenericID	= size_t;
	using ObjectID	= GenericID;
	using TypeID	= GenericID;
	
	static const ObjectID	INVALID_OBJECT_ID = std::numeric_limits<ObjectID>::max();
	static const TypeID		INVALID_TYPE_ID = std::numeric_limits<TypeID>::max();

	using int8	= int8_t;
	using int16 = int16_t;
	using int32 = int32_t;

#ifdef ECS_64BIT
	using int64 = int64_t;
#else
	using int64 = int32_t;
#endif

	using uint8  = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;

#ifdef ECS_64BIT
	using uint64 = uint64_t;
#else
	using uint64 = uint32_t;
#endif

	using float32 = float_t;
	using float64 = double_t;

#define FFE_FMINF(a,b) std::fminf(a,b)
#define FFE_FMAXF(a,b) std::fmaxf(a,b)
#define FFE_FABSF(a) std::fabsf(a)
}

/*
	+-----------------------+---------------+
	|	interpretation		|	  Type		|
	+-----------------------+---------------+
	|	long				|	long int	|
	|	short				|	short int	|
	|	unsigned			|	unsigned int|
	+-----------------------+---------------+	
	|	signed char			|	int8_t		|
	|	short				|	int16_t		|
	|	int					|	int32_t		|
	|	long long			|	int64_t		|
	+-----------------------+---------------+
	|	unsigned char		|	uint8_t		|
	|	unsigned short		|	uint16_t	|
	|	unsigned int		|	uint32_t	|
	|	unsigned long long	|	uint64_t	|
	+-----------------------+---------------+


	+-------------------+-------+
	|		type		|  byte |
	+-------------------+-------+
	|	__int8			|	1	|
	|	bool 			|	1	|
	|	char			|	1	|
	|	unsigned char	|	1	|
	|	signed char		|	1	|
	+-------------------+-------+
	|	__int16 		|	2	|
	|	short			|	2	|
	|	unsigned short	|	2	|
	|	wchar_t			|	2	|
	|	__wchar_t		|	2	|
	+-------------------+-------+
	|	__int32			|	4	|
	|	float			|	4	|
	|	int				|	4	|
	|	unsigned int	|	4	|
	|	long			|	4	|
	|	unsigned long	|	4	|
	+-------------------+-------+
	|	__int64			|	8	|
	|	double			|	8	|
	|	long double		|	8	|
	|	long long		|	8	|
	+-------------------+-------+
	|	__int128		|	16	|
	+-------------------+-------+
*/