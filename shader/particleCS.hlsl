
#include "common.hlsl"

// CS設定
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};


//疑似乱数生成関数///////////////////////////////////////////
float RandomOrig(float2 v)
{
    uint2 u = asint(v * float2(141421356, 2718281828));
    return float((u.x ^ u.y) * 3141592653) * 2.3283064365386962890625e-10;
}
  
float Random(float4 v)
{
    return RandomOrig(v.xy + v.zw);
}
////////////////////////////////////////////////////////////


// In
StructuredBuffer<PARTICLE_LOCAL_CONFIG> BufIn : register(t0);

// Out
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);
RWStructuredBuffer<PARTICLE_GLOBAL_CONFIG_RW> ParticleSettingGlobal : register(u1);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    //ディスパッチ内での一意の値
    int index = input.dispatch.x;
    
        
    //fpsを取得
    float fps = PcInfomation.Fps;

    //fpsの下限を設定
    if (fps < 30.0)
    {
        fps = 30;
    }
    

    //発射許可が出ていない、かつパーティクル自身の経過時間が進んでいなかったら
    if (ParticleGlobalConfigRead.Fireable == 0 && BufIn[index].Life == 0.0)
    {
        return;
    }
    
    
    //パーティクルの生存時間が0.0で発射数が1以上の場合
    if (BufIn[index].Life == 0.0 && ParticleSettingGlobal[0].ParticleShotNum >= 1)
    {
        InterlockedAdd(ParticleSettingGlobal[0].ParticleShotNum, -1);
    }
    else if (BufIn[index].Life == 0.0 && ParticleSettingGlobal[0].ParticleShotNum <= 0)
    { //パーティクルの生存時間が0.0で発射数が0の場合
        return;
    }
        
    //速度を算出
    float3 velocity = BufIn[index].ShootDirection * ParticleGlobalConfigRead.SpeedFactor;
    
    
    //加速度を算出
    float3 acceleration = float3(0.0, 0.0, 0.0);
    
    //重力使用フラグがtrueの場合
    if (ParticleGlobalConfigRead.IsEnableGravity == true)
    { //重力を設定
        acceleration.y -= ParticleGlobalConfigRead.GravityFactor;
    }
    
    
    //速度と加速度による移動量を計算
    float3 result = velocity + acceleration * BufIn[index].Life;
    
    
    //抵抗力の使用がtrueの場合
    if (ParticleGlobalConfigRead.IsEnableDrag == true)
    {
        //抵抗による移動量の減少を計算
        result = result * pow(1.0 - ParticleGlobalConfigRead.DragFactor, BufIn[index].Life);
    }

    //生存時間が寿命を超えた場合
    if (BufIn[index].Life > ParticleGlobalConfigRead.MaxLife)
    {
        //発射方向の乱数を作成
        float3 shootDirection;
        shootDirection.x = Random(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
        shootDirection.y = Random(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
        shootDirection.z = Random(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
        //発射方向を補正
        shootDirection.x = shootDirection.x * 2.0 - 1.0;
        shootDirection.y = shootDirection.y * ParticleGlobalConfigRead.ShootingMethod.x - ParticleGlobalConfigRead.ShootingMethod.y;
        shootDirection.z = shootDirection.z * 2.0 - 1.0;
        
        //発射方向を正規化
        shootDirection = normalize(shootDirection);
        
        //発射方向を初期化
        BufOut[index].ShootDirection = shootDirection;

        //位置を初期化
        BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
        
        //生存時間を初期化
        BufOut[index].Life = 0.0;
    }
    //生存している場合
    else
    {
        //位置情報を格納
        BufOut[index].Position.xyz = BufIn[index].Position.xyz + result;
        BufOut[index].Position.w = 1.0;
        
        //発射方向を格納
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //生存時間を加算して格納
        BufOut[index].Life = BufIn[index].Life + 1.0 / fps;
    }
}
