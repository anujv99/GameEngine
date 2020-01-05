#pragma once

#include <cstdlib>

namespace prev {

	void * PrevMemoryMalloc(std::size_t size, const char * file, std::size_t line);
	void PrevMemoryFree(void * ptr, const char * file, std::size_t line);

}

#define malloc(M) prev::PrevMemoryMalloc(M, __FILE__, __LINE__)
#define free(M) prev::PrevMemoryFree(M, __FILE__, __LINE__)