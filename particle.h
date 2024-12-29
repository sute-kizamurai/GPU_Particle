#pragma once

#include "gameObject.h"

//パーティクル1つ1つの設定
struct PARTICLE_LOCAL_CONFIG
{
	XMFLOAT3 Position; //座標
	XMFLOAT3 ShootDirection; //発射方向
	float SpeedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する
	float MaxLife;//最大寿命
	float Life; //寿命
	float Dummy[3]; //サイズ調整用ダミー
};

//パーティクルエフェクト全体の共通設定
struct PARTICLE_GLOBAL_CONFIG
{
	float SpeedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する
};


class Particle : public GameObject
{
private:
	//コンピュートシェーダ
	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11ComputeShader* m_PingPongShader{};

	//ジオメトリシェーダ
	ID3D11GeometryShader* m_GeometryShader{};

	//パーティクル
	PARTICLE_LOCAL_CONFIG* m_ParticleLocal{};
	PARTICLE_GLOBAL_CONFIG* m_ParticleGlobal{};

	//バッファ
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleLocalBuffer{};
	ID3D11Buffer* m_ResultBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleLocalSRV{};
	ID3D11ShaderResourceView* m_ResultSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ParticleLocalUAV{};
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