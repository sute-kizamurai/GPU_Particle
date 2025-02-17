
#include "common.hlsl"

// CS�ݒ�
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};


//�^�����������֐�///////////////////////////////////////////
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
    //�f�B�X�p�b�`���ł̈�ӂ̒l
    int index = input.dispatch.x;
    
        
    //fps���擾
    float fps = PcInfomation.Fps;

    //fps�̉�����ݒ�
    if (fps < 30.0)
    {
        fps = 30;
    }
    

    //���ˋ����o�Ă��Ȃ��A���p�[�e�B�N�����g�̌o�ߎ��Ԃ��i��ł��Ȃ�������
    if (ParticleGlobalConfigRead.Fireable == 0 && BufIn[index].Life == 0.0)
    {
        return;
    }
    
    //���݂̔��ˉ\��
    int oldNum;

    //���˕������i�[����ϐ�
    float3 shootDirection;
    
    //�p�[�e�B�N���̐������Ԃ�0.0�̏ꍇ
    if (BufIn[index].Life == 0.0)
    {//�p�[�e�B�N���̔��ː�����������������Z
        InterlockedAdd(ParticleSettingGlobal[0].ParticleShotNum, -1, oldNum);
    }
    
    //�p�[�e�B�N���̐������Ԃ�0�ȊO�i�������j
    if (BufIn[index].Life != 0.0)
    {
        shootDirection = BufIn[index].ShootDirection;
    }
    //���݂̔��ː���0���傫���i���˗\��j
    else if (oldNum > 0)
    {//���˗\��̂���p�[�e�B�N���̔��˕�����ݒ�
        //���˕����̗������쐬
        shootDirection.x = Random(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
        shootDirection.y = Random(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
        shootDirection.z = Random(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
        //���˕�����␳
        shootDirection.x = shootDirection.x * 2.0 - 1.0;
        shootDirection.y = shootDirection.y * ParticleGlobalConfigRead.ShootingMethod.x - ParticleGlobalConfigRead.ShootingMethod.y;
        shootDirection.z = shootDirection.z * 2.0 - 1.0;
        
        //���˕����𐳋K��
        shootDirection = normalize(shootDirection);
    }
    else
    { //�p�[�e�B�N���̐������Ԃ�0.0�i���S���j�����˗\�񂪓����Ă��Ȃ�
        return;
    }
        
    
    //���x���Z�o
    float3 velocity = shootDirection * ParticleGlobalConfigRead.SpeedFactor;
    
    
    //�����x���Z�o
    float3 acceleration = float3(0.0, 0.0, 0.0);
    
    //�d�͎g�p�t���O��true�̏ꍇ
    if (ParticleGlobalConfigRead.IsEnableGravity == true)
    { //�d�͂�ݒ�
        acceleration.y -= ParticleGlobalConfigRead.GravityFactor;
    }
    
    
    //���x�Ɖ����x�ɂ��ړ��ʂ��v�Z
    float3 result = velocity + acceleration * BufIn[index].Life;
    
    
    //��R�͂̎g�p��true�̏ꍇ
    if (ParticleGlobalConfigRead.IsEnableDrag == true)
    {
        //��R�ɂ��ړ��ʂ̌������v�Z
        result = result * pow(1.0 - ParticleGlobalConfigRead.DragFactor, BufIn[index].Life);
    }

    //�������Ԃ������𒴂����ꍇ
    if (BufIn[index].Life > ParticleGlobalConfigRead.MaxLife)
    {
        //�ʒu��������
        BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
        
        //�������Ԃ�������
        BufOut[index].Life = 0.0;
    }
    //�������Ă���ꍇ
    else
    {
        //�ʒu�����i�[
        BufOut[index].Position.xyz = BufIn[index].Position.xyz + result;
        BufOut[index].Position.w = 1.0;
        
        //���˕������i�[
        BufOut[index].ShootDirection = shootDirection;
        
        //�������Ԃ����Z���Ċi�[
        BufOut[index].Life = BufIn[index].Life + 1.0 / fps;
    }
}
