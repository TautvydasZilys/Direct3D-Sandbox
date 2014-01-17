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

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^ args)
{
	return 0;
}

#endif