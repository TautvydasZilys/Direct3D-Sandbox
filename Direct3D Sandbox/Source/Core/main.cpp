#include "PrecompiledHeader.h"

#if !WINDOWS_PHONE

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	return 0;
}

#else

#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^ args)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	Windows::ApplicationModel::Core::CoreApplication::Run(direct3DApplicationSource);

	return 0;
}

#endif