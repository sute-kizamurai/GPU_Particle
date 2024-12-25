#pragma once

#include "gameObject.h"


class ParticleGS : public GameObject
{
private:
	//コンピュートシェーダ
	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11ComputeShader* m_PingPongShader{};

	//ジオメトリシェーダ
	ID3D11GeometryShader* m_GeometryShader{};

	//パーティクル
	ParticleCompute* m_Particle{};

	//バッファ
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleSRV{};
	ID3D11ShaderResourceView* m_ResultSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ParticleUAV{};
	ID3D11UnorderedAccessView* m_ResultUAV{};

	//パーティクルサイズ
	XMFLOAT2 m_Size{};

	//パーティクルの最大数
	int m_ParticleAmount{};

	//テクスチャ情報
	ID3D11ShaderResourceView* m_Texture{};

	//パーティクルの変更可能ステータス

	//ライフ
	int m_LifeSlider{};
	//速度
	float m_SpeedSlider{};

	//パーティクルの内容に変更があったか確認
	bool m_ChangeParticle{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};