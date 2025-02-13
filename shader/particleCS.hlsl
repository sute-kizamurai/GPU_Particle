
#include "common.hlsl"

// CS設定
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// In
StructuredBuffer<PARTICLE_LOCAL_CONFIG> BufIn : register(t0);

// Out
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    int index = input.dispatch.x;
        
    //速度を算出
    float3 velocity = BufIn[index].ShootDirection * ParticleGlobalConfig.SpeedFactor;
    
    
    //加速度を算出
    float3 acceleration = float3(0.0, 0.0, 0.0);
    
    //重力使用フラグがtrueの場合
    if (ParticleGlobalConfig.IsEnableGravity == true)
    { //重力を設定
        acceleration.y -= ParticleGlobalConfig.GravityFactor;
    }
    
    
    //速度と加速度による移動量を計算
    float3 result = velocity + acceleration * BufIn[index].Life;
    
    
    //抵抗力の使用がtrueの場合
    if (ParticleGlobalConfig.IsEnableDrag == true)
    {
        //抵抗による移動量の減少を計算
        result = result * pow(1.0 - ParticleGlobalConfig.DragFactor, BufIn[index].Life);
    }
    
    
    if (BufIn[index].Life > ParticleGlobalConfig.MaxLife)
    {
        //位置を初期化
        BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
        
        //生存時間を初期化
        BufOut[index].Life = 0.0;
    }
    else
    {
        //位置情報を格納
        BufOut[index].Position.xyz = BufIn[index].Position.xyz + result;
        BufOut[index].Position.w = 1.0;
        
        //発射方向を格納
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //生存時間を加算して格納
        BufOut[index].Life = BufIn[index].Life + 1.0 / PcInfomation.Fps;
    }
}
