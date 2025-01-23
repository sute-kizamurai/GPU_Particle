
//======================================
// パーティクルの情報を初期化
// Author:中山颯
//======================================

#include "common.hlsl"

// CS設定
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// 出力用バッファ
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    //ディスパッチ内での一意の値を格納
    int index = input.dispatch.x;
    
    //位置の初期化
    BufOut[index].Position = float3(0.0, 0.0, 0.0);
    
    //発射方向の乱数を作成
    
    //速度を初期化
    BufOut[index].Velocity = float3(0.0, 1.0, 0.0) * ParticleGlobalConfig.SpeedFactor;
    
    //加速度を初期化
    BufOut[index].Acceleration = float3(0.0, 0.0, 0.0);
    
    //ライフを初期化
    BufOut[index].Life = ParticleGlobalConfig.MaxLife;

}
