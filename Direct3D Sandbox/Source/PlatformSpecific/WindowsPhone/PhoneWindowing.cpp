#include "PrecompiledHeader.h"
#include "PhoneWindowing.h"

#if WINDOWS_PHONE
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

PhoneWindowing::PhoneWindowing()
{
}

void PhoneWindowing::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &PhoneWindowing::OnActivated);
	CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &PhoneWindowing::OnSuspending);
	CoreApplication::Resuming += ref new EventHandler<Platform::Object^>(this, &PhoneWindowing::OnResuming);
}

void PhoneWindowing::SetWindow(CoreWindow^ window)
{
	window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &PhoneWindowing::OnVisibilityChanged);
	window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &PhoneWindowing::OnWindowClosed);
	window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &PhoneWindowing::OnPointerPressed);
	window->PointerMoved +=	ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &PhoneWindowing::OnPointerMoved);
	window->PointerReleased += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &PhoneWindowing::OnPointerReleased);
}

void PhoneWindowing::Load(Platform::String^ entryPoint)
{
}

void PhoneWindowing::Run()
{
}

void PhoneWindowing::Uninitialize()
{
	CoreApplication::Exit();
}

void PhoneWindowing::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
}

void PhoneWindowing::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
}

void PhoneWindowing::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
}

void PhoneWindowing::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
}

void PhoneWindowing::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
}

void PhoneWindowing::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void PhoneWindowing::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
}
 
void PhoneWindowing::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new PhoneWindowing();
}

#endif	// WINDOWS_PHONE