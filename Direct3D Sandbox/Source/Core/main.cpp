#include "PrecompiledHeader.h"
#include "CoInitializeWrapper.h"
#include "System.h"

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
	System system;
	system.Run();

	return 0;
}