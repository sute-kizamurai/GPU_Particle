
#include "common.hlsl"

struct Particle
{
    float3 position; //座標
    float3 shootDirection; //発射方向
    float speedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する
    float maxLife; //最大寿命
    float life; //寿命
    float dummy[3]; //サイズ調整用ダミー
};

StructuredBuffer<Particle> particle : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = mul(In.Position, World);
	
    Out.Position.xyz += particle[In.InstanceId].position;
    
    //ワールド変換した頂点座標を出力
    Out.WorldPosition = mul(Out.Position, World);

    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

