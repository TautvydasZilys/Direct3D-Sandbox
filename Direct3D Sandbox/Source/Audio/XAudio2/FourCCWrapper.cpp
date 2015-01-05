#include "PrecompiledHeader.h"
#include "FourCCWrapper.h"


FourCCWrapper::FourCCWrapper(int fourCC)
{
	*reinterpret_cast<int*>(cc) = fourCC;
}

FourCCWrapper::~FourCCWrapper()
{
}
