#pragma once

////インクルード
#include "gameObject.h"


////構造体宣言
//パーティクル個別の設定
struct PARTICLE_LOCAL_CONFIG
{
	XMFLOAT4 Position; //座標
	XMFLOAT3 ShootDirection; //発射方向
	float Life; //生存時間
};

//パーティクルエフェクト全体の共通設定（GPU内で読み込み専用）
struct PARTICLE_GLOBAL_CONFIG_R
{
	XMFLOAT2 ShootingMethod; //発射方向を決定するための補正値を格納

	int Fireable; //パーティクルが発射可能かどうかを送る(発射可能は1、発射不可能は0)

	float MaxLife; //パーティクルの最大寿命

	float SpeedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する

	BOOL IsEnableGravity; //重力を使用するかどうかのフラグ
	float GravityFactor; //重力の強さ

	BOOL IsEnableDrag; //抵抗力を使用するかどうかのフラグ
	float DragFactor; //抵抗力の強さ
	
	float DummyFloat; //サイズ調整用ダミー(float型)
	BOOL DummyBool[2]; //サイズ調整用ダミー(bool型)
};

//パーティクルエフェクト全体の共通設定（GPU内で読み書き可能）
struct PARTICLE_GLOBAL_CONFIG_RW
{
	int ShotNum; //一度に発射できるパーティクルの数
	float DummyFloat[3]; //サイズ調整用ダミー(float型)
};


class Particle : public GameObject
{
private:
	//コンピュートシェーダ
	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11ComputeShader* m_ParticleInitialShader{};
	ID3D11ComputeShader* m_PingPongShader{};

	//ジオメトリシェーダ
	ID3D11GeometryShader* m_GeometryShader{};

	//パーティクル
	PARTICLE_LOCAL_CONFIG* m_ParticleLocal{};
	PARTICLE_GLOBAL_CONFIG_R* m_ParticleGlobalRead{};
	PARTICLE_GLOBAL_CONFIG_RW* m_ParticleGlobalReadWrite{};

	//バッファ
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleLocalBuffer{};
	ID3D11Buffer* m_ParticleGlobalReadBuffer{}; //GPU内で読み込み専用のバッファ
	ID3D11Buffer* m_ParticleGlobalReadWriteBuffer{}; //GPU内で読み書き可能なバッファ
	ID3D11Buffer* m_ResultBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleLocalSRV{};
	ID3D11ShaderResourceView* m_ResultSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ParticleLocalUAV{};
	ID3D11UnorderedAccessView* m_ResultUAV{};
	ID3D11UnorderedAccessView* m_m_ParticleGlobalReadWriteUAV{};

	//パーティクルサイズ
	XMFLOAT2 m_Size{};

	//パーティクルの最大数
	int m_ParticleAmount{};

	//テクスチャ情報
	ID3D11ShaderResourceView* m_Texture{};

	//パーティクルの色情報
	XMFLOAT4 m_ParticleColor{};

	//パーティクルの内容に変更があったか確認
	bool m_ChangeParticle{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//パーティクルの個別設定を生成する
	//IN ParticleAmount : パーティクルの発射数を生成
	void CreateParticleMaxCapacity(int ParticleAmount);

	//パーティクルの全体設定を生成する
	void CreateParticleGlobal();

public://セッター＆ゲッター
	int GetParticleAmount() { return m_ParticleAmount; }

	void SetMaxLife(float MaxLife)
	{ 
		m_ParticleGlobalRead->MaxLife = MaxLife;
		m_ChangeParticle = true;
	}
	void SetSpeedFactor(float SpeedFactor)
	{
		m_ParticleGlobalRead->SpeedFactor = SpeedFactor;
		m_ChangeParticle = true;
	}
	void SetIsEnableGravity(bool IsEnableGravity)
	{
		m_ParticleGlobalRead->IsEnableGravity = IsEnableGravity;
		m_ChangeParticle = true;
	}
	void SetGravityFactor(float GravityFactor)
	{
		m_ParticleGlobalRead->GravityFactor = GravityFactor;
		m_ChangeParticle = true;
	}
	void SetIsEnableDrag(bool IsEnableDrag)
	{
		m_ParticleGlobalRead->IsEnableDrag = IsEnableDrag;
		m_ChangeParticle = true;
	}
	void SetDragFactor(float DragFactor)
	{
		m_ParticleGlobalRead->DragFactor = DragFactor;
		m_ChangeParticle = true;
	}
	void SetShootingMethod(XMFLOAT2 ShootingMethod)
	{
		m_ParticleGlobalRead->ShootingMethod = ShootingMethod;
		m_ChangeParticle = true;
	}
	void SetFireable(int Fireable)
	{
		m_ParticleGlobalRead->Fireable = Fireable;
		m_ChangeParticle = true;
	}
	void SetShotNum(int ShotNum)
	{
		m_ParticleGlobalReadWrite->ShotNum = ShotNum;
	}


	void SetParticleColor(XMFLOAT4 Color)
	{
		m_ParticleColor = Color;
	}
};