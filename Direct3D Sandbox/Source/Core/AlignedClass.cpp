#include "PrecompiledHeader.h"
#include "AlignedClass.h"

void* AlignedClass::operator new(size_t size)
{
	void* ptr = _aligned_malloc(size, 16);

	if (ptr == nullptr)
	{
		throw bad_alloc();
	}

	return ptr;
}

void AlignedClass::operator delete(void* p)
{
	_aligned_free(p);
}