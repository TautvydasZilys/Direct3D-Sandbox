#pragma once

#include "PrecompiledHeader.h"

class Tools
{
private:
	Tools();
	~Tools();
	
	static long long int m_PerformanceCounterFrequency;
	
	inline static int InitPerformanceCounterFrequency()
	{
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_PerformanceCounterFrequency));
		return 0;
	}

public:	
	static inline float GetTime()
	{
		static int dummy = InitPerformanceCounterFrequency();
		long long int timer;

		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timer));
		return static_cast<float>(timer) / static_cast<float>(m_PerformanceCounterFrequency);
	}
};

struct Point2D
{
	float x, y;

	Point2D() {}
	Point2D(float x, float y) : x(x), y(y) {}
};

#if !DEBUG
#define Assert(x)	(void)(x)
#elif WINDOWS_PHONE
#define Assert(x)   do \
					{ \
						if (!(x)) \
						{ \
							OutputDebugStringW(L"Fatal Error!\r\n"); \
							OutputDebugStringW((__WFILE__ + wstring(L": ") + to_wstring(__LINE__) + L"\r\n").c_str()); \
							__debugbreak(); \
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

