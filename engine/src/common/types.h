#pragma once

#include <cstdint>
#include <string>

#define _S(S) #S

namespace prev {

	typedef char pvchar;
	typedef unsigned char pvuchar;

	typedef float pvfloat;

	typedef int32_t pvint;
	typedef uint32_t pvuint;
	typedef size_t pvsizet;

	typedef bool pvbool;

	typedef std::string pvstring;

	typedef uintptr_t pvptr;
}