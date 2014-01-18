#pragma once

class Tools
{
private:
	Tools();
	~Tools();

public:
	

};


#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define __WFILE__ WIDE1(__FILE__)

#if !DEBUG
#define Assert(x)	(void)(x)
#elif WINDOWS_PHONE
#define Assert(x)   do \
					{ \
						if (!(x)) \
						{ \
							OutputDebugStringW(L"Fatal Error!\r\n"); \
							OutputDebugStringW((__WFILE__ + wstring(L": ") + to_wstring(__LINE__) + L"\r\n").c_str()); \
							assert(false); \
						} \
					} \
					while (false)
#else
#define Assert(x)   do \
					{ \
						if (!(x)) \
						{ \
							OutputDebugStringW(L"Fatal Error!\r\n"); \
							OutputDebugStringW((__WFILE__ + wstring(L": ") + to_wstring(__LINE__)).c_str()); \
							DebugBreak(); \
						} \
					} \
					while (false)
#endif

