#pragma once

#include "gameObject.h"

class ComputeShader : public GameObject
{
private:
	//コンピュートシェーダ―
	ID3D11ComputeShader* m_ComputeShader;

	//パーティクル
	ParticleCompute* m_Particle{};

	//バッファ
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};
	ID3D11Buffer* m_PositionBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleSRV{};
	ID3D11ShaderResourceView* m_PositionSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ResultUAV{};

	//パーティクルサイズ
	XMFLOAT2 m_Size{};

	//パーティクルの最大数
	int m_ParticleAmount{};

	//テクスチャ情報
	ID3D11ShaderResourceView* m_Texture{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};