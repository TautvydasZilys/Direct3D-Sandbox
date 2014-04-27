#pragma once

class AlignedClass
{
public:
	void* operator new(size_t size);
	void operator delete(void* p);
};

