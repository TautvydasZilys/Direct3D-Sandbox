#include "PrecompiledHeader.h"
#include "CoInitializeWrapper.h"
#include "System.h"

#if !WINDOWS_PHONE

int CALLBACK WinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR lpCmdLine,
  _In_ int nCmdShow
)
{
#if DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif
	CoInitializeWrapper coInitialize;

	System system;
	system.Run();

	return 0;
}

#else

#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^ args)
{
#if DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	Windows::ApplicationModel::Core::CoreApplication::Run(direct3DApplicationSource);

	return 0;
}

#endif