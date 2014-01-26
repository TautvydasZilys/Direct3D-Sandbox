#pragma once

class SamplerState
{
private:
	static unordered_map<string, ComPtr<ID3D11SamplerState>> s_SamplerStates;
	
#if DEBUG
	static bool s_Initialized;
#endif

	SamplerState();
	~SamplerState();

public:
	static void Initialize(ComPtr<ID3D11Device> device);
	static ComPtr<ID3D11SamplerState> Get(const string& name);
};

