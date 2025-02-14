
#include "common.hlsl"

// CSİ’è
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};


//‹^——”¶¬ŠÖ”///////////////////////////////////////////
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

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    int index = input.dispatch.x;
        
    //‘¬“x‚ğZo
    float3 velocity = BufIn[index].ShootDirection * ParticleGlobalConfig.SpeedFactor;
    
    
    //‰Á‘¬“x‚ğZo
    float3 acceleration = float3(0.0, 0.0, 0.0);
    
    //d—Íg—pƒtƒ‰ƒO‚ªtrue‚Ìê‡
    if (ParticleGlobalConfig.IsEnableGravity == true)
    { //d—Í‚ğİ’è
        acceleration.y -= ParticleGlobalConfig.GravityFactor;
    }
    
    
    //‘¬“x‚Æ‰Á‘¬“x‚É‚æ‚éˆÚ“®—Ê‚ğŒvZ
    float3 result = velocity + acceleration * BufIn[index].Life;
    
    
    //’ïR—Í‚Ìg—p‚ªtrue‚Ìê‡
    if (ParticleGlobalConfig.IsEnableDrag == true)
    {
        //’ïR‚É‚æ‚éˆÚ“®—Ê‚ÌŒ¸­‚ğŒvZ
        result = result * pow(1.0 - ParticleGlobalConfig.DragFactor, BufIn[index].Life);
    }
    
    
    //fps‚ğæ“¾
    float fps = PcInfomation.Fps;

    if (fps < 30.0)
    {
        fps = 30;
    }
    
    //¶‘¶ŠÔ‚ªõ–½‚ğ’´‚¦‚½ê‡
    if (BufIn[index].Life > ParticleGlobalConfig.MaxLife)
    {
        //”­Ë•ûŒü‚Ì—”‚ğì¬
        float3 shootDirection;
        shootDirection.x = Random(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
        shootDirection.y = Random(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
        shootDirection.z = Random(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
        //”­Ë•ûŒü‚ğ•â³
        shootDirection.x = shootDirection.x * 2.0 - 1.0;
        shootDirection.y = shootDirection.y * ParticleGlobalConfig.ShootingMethod.x - ParticleGlobalConfig.ShootingMethod.y;
        shootDirection.z = shootDirection.z * 2.0 - 1.0;
        
        //”­Ë•ûŒü‚ğ³‹K‰»
        shootDirection = normalize(shootDirection);
        
        //”­Ë•ûŒü‚ğ‰Šú‰»
        BufOut[index].ShootDirection = shootDirection;

        //ˆÊ’u‚ğ‰Šú‰»
        BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
        
        //¶‘¶ŠÔ‚ğ‰Šú‰»
        BufOut[index].Life = 0.0;
    }
    //¶‘¶‚µ‚Ä‚¢‚éê‡
    else
    {
        //ˆÊ’uî•ñ‚ğŠi”[
        BufOut[index].Position.xyz = BufIn[index].Position.xyz + result;
        BufOut[index].Position.w = 1.0;
        
        //”­Ë•ûŒü‚ğŠi”[
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //¶‘¶ŠÔ‚ğ‰ÁZ‚µ‚ÄŠi”[
        BufOut[index].Life = BufIn[index].Life + 1.0 / fps;
    }
}
