#include "PrecompiledHeader.h"
#include "CoInitializeWrapper.h"
#include "Tools.h"


CoInitializeWrapper::CoInitializeWrapper()
{
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	Assert(result == S_OK);
}

CoInitializeWrapper::~CoInitializeWrapper()
{
	CoUninitialize();
}
