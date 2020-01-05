#include "memory.h"

#include <new>
#include <memory>

#pragma warning(push)
#pragma warning(disable:4290)

void * operator new(std::size_t size) noexcept(false) { return prev::PrevMemoryMalloc(size, "", 0); }
void * operator new(std::size_t size, const std::nothrow_t & nothrowValue) throw() { return prev::PrevMemoryMalloc(size, "", 0); }

void operator delete(void * ptr) throw() { return prev::PrevMemoryFree(ptr, "", 0); }
void operator delete(void * ptr, const std::nothrow_t & nothrow_constant) throw() { return prev::PrevMemoryFree(ptr, "", 0); }

void * operator new[](std::size_t size) noexcept(false) { return prev::PrevMemoryMalloc(size, "", 0); }
void * operator new[](std::size_t size, const std::nothrow_t & nothrow_value) throw() { return prev::PrevMemoryMalloc(size, "", 0); }

void operator delete[](void * ptr) throw() { return prev::PrevMemoryFree(ptr, "", 0); }
void operator delete[](void * ptr, const std::nothrow_t & nothrow_constant) throw() { return prev::PrevMemoryFree(ptr, "", 0); }

#pragma warning(pop)

void * prev::PrevMemoryMalloc(std::size_t size, const char * file, std::size_t line) {
	return (malloc)(size);
}

void prev::PrevMemoryFree(void * ptr, const char * file, std::size_t line) {
	(free)(ptr);
}
