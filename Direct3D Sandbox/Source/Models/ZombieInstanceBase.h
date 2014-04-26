#pragma once

#include "ModelInstance.h"

class PlayerInstance;
class ZombieInstanceBase :
	public ModelInstance
{	
protected:
	const PlayerInstance& m_TargetPlayer;

	ZombieInstanceBase(IShader& shader, const wstring& modelPath, const wstring& texturePath, const wstring& normalMapPath,
		const ModelParameters& modelParameters, const PlayerInstance& targetPlayer);

	static ModelParameters GetRandomZombieParameters(const PlayerInstance& targetPlayer);

private:
	ZombieInstanceBase(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstanceBase& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	virtual ~ZombieInstanceBase();
	virtual void UpdateAndRender(RenderParameters& renderParameters);
};
