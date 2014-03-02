#pragma once

struct ICorRuntimeHost;
namespace mscorlib { struct _AppDomain; }

class ManagedInvoker
{
private:	
	static ComPtr<ICorRuntimeHost> s_RuntimeHost;
	static ComPtr<mscorlib::_AppDomain> s_AppDomain;

public:
	ManagedInvoker();
	~ManagedInvoker();

	void Execute(const wstring& assembly, const wstring& type, const wstring& method, SAFEARRAY* arguments);
};

