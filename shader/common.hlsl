


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}


struct CAMERA
{
    float3 Position;
    float3 ForwardVector;
    float3 RightVector;
    float3 UpVector;
};

cbuffer CameraBuffer : register(b5)
{
    CAMERA Camera;
}


struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
    uint InstanceId		: SV_InstanceID;
};

struct GS_IN
{
    float4 Position			: SV_POSITION;
    float4 WorldPosition	: POSITION0;
    float4 Diffuse			: COLOR0;
    float2 TexCoord			: TEXCOORD0;
    float Clip              : SV_ClipDistance0;
};

struct PS_IN
{
	float4 Position			: SV_POSITION;
    float4 WorldPosition	: POSITION0;
    float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;
    float Clip              : SV_ClipDistance0;
};


// パーティクル個別用構造体
struct PARTICLE_LOCAL_CONFIG
{
    float4 Position; //座標
    float3 ShootDirection; //発射方向
    float Life; //寿命
};


//パーティクル全体設定用構造体（読み込み専用）
struct PARTICLE_GLOBAL_CONFIG_R
{
    float2 ShootingMethod; //発射方向を決定するための補正値を格納
    int Fireable; //パーティクルが発射可能かどうかを送る(発射可能は1、発射不可能は0)
    float MaxLife; //パーティクルの最大寿命    
    float SpeedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する
    bool IsEnableGravity; //重力を使用するかどうかのフラグ
    float GravityFactor; //重力の強さ
    bool IsEnableDrag; //抵抗力を使用するかどうかのフラグ
    float DragFactor; //抵抗力の強さ
    float2 DummyFloat; //サイズ調整用ダミー(float型)
    bool2 DummyBool; //サイズ調整用ダミー(bool型)
};

cbuffer ParticleGlobalBuffer : register(b6)
{
    PARTICLE_GLOBAL_CONFIG_R ParticleGlobalConfigRead;
}

//パーティクル全体設定用構造体（読み書き可能）
struct PARTICLE_GLOBAL_CONFIG_RW
{
    int ParticleShotNum; //一度に発射できるパーティクルの数
    float3 DummyFloat; //サイズ調整用ダミー(float型)
};

//PC情報用構造体
struct PC_INFOMATION
{
    float Fps;
    float3 Dummy;
};

cbuffer ParticleGlobalBuffer : register(b7)
{
    PC_INFOMATION PcInfomation;
}
