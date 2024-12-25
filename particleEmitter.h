#pragma once

#include "gameObject.h"

class ParticleEmitter : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	int m_Count{};

	struct PARTICLE
	{
		bool Enable;
		int Life;
		XMFLOAT3 Position;
		XMFLOAT3 Velocity;
	};

	static const int PARTICLE_MAX = 8000;
	PARTICLE m_Particle[PARTICLE_MAX]{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
