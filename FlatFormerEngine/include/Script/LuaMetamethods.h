#pragma once

namespace FlatFormer { namespace Script {
	
	// Mathematical operations
	static constexpr const char * LMM__unm			{"__unm"};			// -a		(self: value)				unary minus
	static constexpr const char * LMM__add			{"__add"};			// a + b	(op1: value, op2: value)	addition
	static constexpr const char * LMM__sub			{"__sub"};			// a - b	(op1: value, op2: value)	substraction
	static constexpr const char * LMM__mul			{"__mul"};			// a * b	(op1: value, op2: value)	multiplication
	static constexpr const char * LMM__div			{"__div"};			// a / b	(op1: value, op2: value)	division
	static constexpr const char * LMM__mod			{"__mod"};			// a % b	(op1: value, op2: value)	remainder of the division
	static constexpr const char * LMM__pow			{"__pow"};			// a ^ b	(op1: value, op2: value)	exponentiation
	static constexpr const char * LMM__idiv			{"__idiv"};			// a // b	(op1: value, op2: value)	integer division (Lua 5.3)
	
	// Comparison operators
	static constexpr const char * LMM__eq			{"__eq"};			// a == b	(op1: value, op2: value)	//	"~="	inverts "=="
	static constexpr const char * LMM__lt			{"__lt"};			// a < b	(op1: value, op2: value)	//	">"		swaps operands
	static constexpr const char * LMM__le			{"__le"};			// a <= b	(op1: value, op2: value)	//	">="	swaps operands
	
	// Bit operators
	static constexpr const char * LMM__band			{"__band"};			// &		(op1: value, op2: value)	bitwise AND
	static constexpr const char * LMM__bor			{"__bor"};			// |		(op1: value, op2: value)	bitwise OR
	static constexpr const char * LMM__bxor			{"__bxor"};			// ~		(op1: value, op2: value)	bitwise XOR
	static constexpr const char * LMM__bnot			{"__bnot"};			// ~		(self: value)				bitwise NOT
	static constexpr const char * LMM__shl			{"__shl"};			// <<		(op1: value, op2: value)	bit shift LEFT
	static constexpr const char * LMM__shr			{"__shr"};			// >>		(op1: value, op2: value)	bit shift RIGHT
	
	// Other
	static constexpr const char * LMM__index		{"__index"};		// (self: value, key: value)					called when indexing
	static constexpr const char * LMM__newindex		{"__newindex"};		// (self: value, key : value, value : value)	called when the value under the index changes
	static constexpr const char * LMM__call			{"__call"};			// (self: value, args: ... )					fires when the table is accessed as a function
	static constexpr const char * LMM__metatable	{"__metatable"};	// value										metatable field returned by 'getmetatable(value: value):value'
	static constexpr const char * LMM__tostring		{"__tostring"};		// (self: value)								called when trying to convert a value to a string
	static constexpr const char * LMM__concat		{ "__concat" };		// (op1: value, op2: value)						concatenation operation 'a .. b'
	static constexpr const char * LMM__len			{"__len"};			// (self: value)								called when using the '#' operator
	static constexpr const char * LMM__pairs		{"__pairs"};		// (self: value)								replaces the 'pairs(table: table)' function return statement
	static constexpr const char * LMM__ipairs		{"__ipairs"};		// (self: value)								replaces the 'ipairs(table: table)' function return statement
	static constexpr const char * LMM__name			{"__name"};			// string										metafield, used when an error occurs
	static constexpr const char * LMM__gc			{"__gc"};			// (self: value)								called when the garbage collector wants to delete an object
	static constexpr const char * LMM__mode			{"__mode"};			// string										parameter indicating that the table cannot protect keys/values from __gc
	
	//example"
	//using "__metatable" you can hide the metatable (close access to it)
	// foo = setmetatable({}, mt)
	// mt = {}
	// mt.__metatable = "No metatables here!"
	// getmetatable(foo) --> "No metatables here!"
	
	// LUA_TNONE               (-1)
	// 
	// LUA_TNIL                0
	// LUA_TBOOLEAN            1
	// LUA_TLIGHTUSERDATA      2
	// LUA_TNUMBER             3
	// LUA_TSTRING             4
	// LUA_TTABLE              5
	// LUA_TFUNCTION           6
	// LUA_TUSERDATA           7
	// LUA_TTHREAD             8
	// 
	// LUA_NUMTAGS             9

}}