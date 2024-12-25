#pragma once

#include "gameObject.h"

//パーティクルの資料例
struct ParticleCompute
{
	XMFLOAT3 Position; //座標
	XMFLOAT3 ShootDirection; //発射方向
	float SpeedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する
	float MaxLife;//最大寿命
	float Life; //寿命
	float Dummy[3]; //サイズ調整用ダミー
};

class Particle : public GameObject
{
private:
	//コンピュートシェーダ―
	ID3D11ComputeShader* m_ComputeShader{};

	//パーティクル
	ParticleCompute* m_Particle{};

	//バッファ
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};
	ID3D11Buffer* m_PositionBuffer{};
	ID3D11Buffer* m_StagingBuffer{};

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

	float m_SliderValue{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};