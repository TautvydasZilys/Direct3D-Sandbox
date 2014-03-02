#include "PrecompiledHeader.h"
#include "ManagedInvoker.h"

#include <metahost.h>

#import "C:\Windows\Microsoft.NET\Framework\v4.0.30319\mscorlib.tlb" raw_interfaces_only
using namespace mscorlib;

#include "Tools.h"	// mscorlib.tlb doesn't like Assert macro

ComPtr<ICorRuntimeHost> ManagedInvoker::s_RuntimeHost;
ComPtr<_AppDomain> ManagedInvoker::s_AppDomain;

ManagedInvoker::ManagedInvoker()
{
	if (s_RuntimeHost != nullptr)
	{
		return;
	}

	HRESULT result;
	ComPtr<ICLRMetaHost> metaHost;
    ComPtr<ICLRRuntimeInfo> runtimeInfo;
	ComPtr<IUnknown> appDomainObj;

	result = CLRCreateInstance(CLSID_CLRMetaHost, __uuidof(ICLRMetaHost), &metaHost);
	Assert(result == S_OK);

	result = metaHost->GetRuntime(L"v4.0.30319", __uuidof(ICLRRuntimeInfo), &runtimeInfo);
	Assert(result == S_OK);

	result = runtimeInfo->GetInterface(CLSID_CorRuntimeHost, __uuidof(ICorRuntimeHost), &s_RuntimeHost);
	Assert(result == S_OK);

	result = s_RuntimeHost->Start();
	Assert(result == S_OK);

	result = s_RuntimeHost->GetDefaultDomain(&appDomainObj);
	Assert(result == S_OK);
	
	result = appDomainObj.As(&s_AppDomain);
	Assert(result == S_OK);
}

ManagedInvoker::~ManagedInvoker()
{
}

void ManagedInvoker::Execute(const wstring& assembly, const wstring& type, const wstring& method, SAFEARRAY* arguments)
{
	HRESULT result;
	ComPtr<_Assembly> loadedAssembly;
	ComPtr<_Type> loadedType;
	SAFEARRAYBOUND bounds;
	void* rawData;
	
	bstr_t typeName(type.c_str());
	bstr_t methodName(method.c_str());

	auto rawAssembly = Tools::ReadFileToVector(assembly);    
    bounds.lLbound = 0;
	bounds.cElements = static_cast<ULONG>(rawAssembly.size());
	
	auto rawAssemblyArray = SafeArrayCreate(VT_UI1, 1, &bounds);
	Assert(rawAssemblyArray != nullptr);

	result = SafeArrayAccessData(rawAssemblyArray, &rawData);
	Assert(result == S_OK);

	memcpy(rawData, rawAssembly.data(), rawAssembly.size());

	result = SafeArrayUnaccessData(rawAssemblyArray);
	Assert(result == S_OK);

	result = s_AppDomain->Load_3(rawAssemblyArray, &loadedAssembly);
	Assert(result == S_OK);

	result = loadedAssembly->GetType_2(typeName, &loadedType);
	Assert(result == S_OK);
	Assert(loadedType != nullptr);

	auto bindingFlags = static_cast<BindingFlags>(BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public);
	variant_t emptyTarget, returnValue;

	result = loadedType->InvokeMember_3(methodName, bindingFlags, nullptr, emptyTarget, arguments, &returnValue);
	Assert(result == S_OK);

	result = SafeArrayDestroy(rawAssemblyArray);
	Assert(result == S_OK);
}