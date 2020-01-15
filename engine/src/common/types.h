#pragma once

#include <cstdint>

#include "../utils/prevstring.h"

#define _S(S) #S

namespace prev {

	typedef char pvchar;
	typedef unsigned char pvuchar;

	typedef float pvfloat;
	typedef double pvdouble;

	typedef int8_t pvbyte;
	typedef uint8_t pvubyte;

	typedef int16_t pvshort;
	typedef uint16_t pvushort;

	typedef int32_t pvint;
	typedef uint32_t pvuint;

	typedef int64_t pvint64;
	typedef uint64_t pvuint64;

	typedef size_t pvsizet;

	typedef bool pvbool;

	typedef PrevString pvstring;

	typedef uintptr_t pvptr;
}