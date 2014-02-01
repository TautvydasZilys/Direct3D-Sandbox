#include "PrecompiledHeader.h"
#include "PhoneWindowing.h"
#include "Tools.h"
#include "System.h"

#if WINDOWS_PHONE
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Phone::UI::Input;
using namespace Windows::System;
using namespace Windows::System::Display;
using namespace Windows::UI::Core;

struct __declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F")) __declspec(novtable)
ICoreWindowInterop : IUnknown
{
    virtual HRESULT __stdcall get_WindowHandle(HWND * hwnd) = 0;
    virtual HRESULT __stdcall put_MessageHandled(unsigned char) = 0;
};

PhoneWindowing::PhoneWindowing() :
	m_WindowHandle(nullptr), m_DisplayRequest(ref new DisplayRequest)
{
	ComPtr<ICoreWindowInterop> windowInterop;
	
	auto window = CoreWindow::GetForCurrentThread();
	
	auto result = reinterpret_cast<IUnknown*>(window)->QueryInterface(windowInterop.GetAddressOf());
	Assert(result == S_OK);

	auto scale = Windows::Graphics::Display::DisplayProperties::LogicalDpi / 96.0f;
	m_Width = static_cast<int>(window->Bounds.Width * scale + 0.5f);
	m_Height = static_cast<int>(window->Bounds.Height * scale + 0.5f);

	result = windowInterop->get_WindowHandle(&m_WindowHandle);
	Assert(result == S_OK);

	m_DisplayRequest->RequestActive();
}

PhoneWindowing::~PhoneWindowing()
{
	m_DisplayRequest->RequestRelease();
}

void PhoneWindowing::DispatchMessages() const
{
	CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent);
	if (Input::GetInstance().IsPaused())
	{
		CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessOneAndAllPending);
	}
}

PhoneFrameworkView::PhoneFrameworkView()
{
}

void PhoneFrameworkView::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &PhoneFrameworkView::OnActivated);
	CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &PhoneFrameworkView::OnSuspending);
	CoreApplication::Resuming += ref new EventHandler<Platform::Object^>(this, &PhoneFrameworkView::OnResuming);
	HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs^>(this, &PhoneFrameworkView::OnBackButtonPressed);
}

void PhoneFrameworkView::SetWindow(CoreWindow^ window)
{
	window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &PhoneFrameworkView::OnVisibilityChanged);
	window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &PhoneFrameworkView::OnWindowClosed);
	window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &PhoneFrameworkView::OnPointerPressed);
	window->PointerMoved +=	ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &PhoneFrameworkView::OnPointerMoved);
	window->PointerReleased += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &PhoneFrameworkView::OnPointerReleased);
	window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &PhoneFrameworkView::OnKeyDown);
	window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &PhoneFrameworkView::OnKeyUp);
}

void PhoneFrameworkView::Load(Platform::String^ entryPoint)
{
}

void PhoneFrameworkView::Run()
{
	System system;
	system.Run();
}

void PhoneFrameworkView::Uninitialize()
{
	CoreApplication::Exit();
}

void PhoneFrameworkView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	Input::GetInstance().SetPause(!args->Visible);
}

void PhoneFrameworkView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
}

void PhoneFrameworkView::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	int id = args->CurrentPoint->PointerId;

	cursorPosition[id] = Point2D(args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
	numberOfFingersDown++;
}

void PhoneFrameworkView::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	auto& input = Input::GetInstance();
	int id = args->CurrentPoint->PointerId;

	if (numberOfFingersDown == 1)
	{
		input.SetMouseDisplacement(static_cast<long int>(cursorPosition[id].x - args->CurrentPoint->RawPosition.X), 
								   static_cast<long int>(cursorPosition[id].y - args->CurrentPoint->RawPosition.Y));
	}
	else if (numberOfFingersDown == 2)
	{
		int id2;

		for (const auto& it : cursorPosition)
		{
			if (it.first != id)
			{
				id2 = it.first;
				break;
			}
		}
		
		float oldDistanceX = cursorPosition[id].x - cursorPosition[id2].x;
		float oldDistanceY = cursorPosition[id].y - cursorPosition[id2].y;

		float newDistanceX = args->CurrentPoint->RawPosition.X - cursorPosition[id2].x;
		float newDistanceY = args->CurrentPoint->RawPosition.Y - cursorPosition[id2].y;

		float distanceDelta = sqrt(newDistanceX * newDistanceX + newDistanceY * newDistanceY) - sqrt(oldDistanceX * oldDistanceX + oldDistanceY * oldDistanceY);
		input.SetPinchDisplacement(static_cast<long int>(distanceDelta));
	}
	
	cursorPosition[id].x = args->CurrentPoint->RawPosition.X;
	cursorPosition[id].y = args->CurrentPoint->RawPosition.Y;
}

void PhoneFrameworkView::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	cursorPosition.erase(args->CurrentPoint->PointerId);
	numberOfFingersDown--;
}

void PhoneFrameworkView::OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{
	Input::GetInstance().KeyDown(static_cast<int>(args->VirtualKey));
	args->Handled = true;
}

void PhoneFrameworkView::OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{
	Input::GetInstance().KeyUp(static_cast<int>(args->VirtualKey));
	args->Handled = true;
}

void PhoneFrameworkView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void PhoneFrameworkView::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
}
 
void PhoneFrameworkView::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
}

void PhoneFrameworkView::OnBackButtonPressed(Platform::Object^ sender, BackPressedEventArgs^ args)
{
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new PhoneFrameworkView;
}

#endif	// WINDOWS_PHONE