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
	typedef int32_t pvint;
	typedef int64_t pvint64;
	typedef uint8_t pvubyte;
	typedef uint32_t pvuint;
	typedef size_t pvsizet;

	typedef bool pvbool;

	typedef PrevString pvstring;

	typedef uintptr_t pvptr;
}