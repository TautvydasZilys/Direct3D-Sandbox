#include "PrecompiledHeader.h"
#include "System.h"

System::System() :
	m_Input(Input::GetInstance())
{
}

System::~System()
{
}

void System::Run()
{
	while (!m_Input.ShouldQuit())
	{
		m_Windowing.DispatchMessages();
	}
}