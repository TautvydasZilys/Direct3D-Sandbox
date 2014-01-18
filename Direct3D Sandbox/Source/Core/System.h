#pragma once

#include "Source\PlatformSpecific\Windows\DesktopWindowing.h"
#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

class System
{
private:
#if !WINDOWS_PHONE
	DesktopWindowing m_Windowing;
#else
	PhoneWindowing m_Windowing;
#endif

public:
	System();
	~System();

	void Run();
};

