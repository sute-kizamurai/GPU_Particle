


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


// �p�[�e�B�N���ʗp�\����
struct PARTICLE_LOCAL_CONFIG
{
    float4 Position; //���W
    float3 ShootDirection; //���˕���
    float Life; //����
};


//�p�[�e�B�N���S�̐ݒ�p�\���́i�ǂݍ��ݐ�p�j
struct PARTICLE_GLOBAL_CONFIG_R
{
    float2 ShootingMethod; //���˕��������肷�邽�߂̕␳�l���i�[
    int Fireable; //�p�[�e�B�N�������ˉ\���ǂ����𑗂�(���ˉ\��1�A���˕s�\��0)
    float MaxLife; //�p�[�e�B�N���̍ő����    
    float SpeedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����
    bool IsEnableGravity; //�d�͂��g�p���邩�ǂ����̃t���O
    float GravityFactor; //�d�͂̋���
    bool IsEnableDrag; //��R�͂��g�p���邩�ǂ����̃t���O
    float DragFactor; //��R�͂̋���
    float2 DummyFloat; //�T�C�Y�����p�_�~�[(float�^)
    bool2 DummyBool; //�T�C�Y�����p�_�~�[(bool�^)
};

cbuffer ParticleGlobalBuffer : register(b6)
{
    PARTICLE_GLOBAL_CONFIG_R ParticleGlobalConfigRead;
}

//�p�[�e�B�N���S�̐ݒ�p�\���́i�ǂݏ����\�j
struct PARTICLE_GLOBAL_CONFIG_RW
{
    int ParticleShotNum; //��x�ɔ��˂ł���p�[�e�B�N���̐�
    float3 DummyFloat; //�T�C�Y�����p�_�~�[(float�^)
};

//PC���p�\����
struct PC_INFOMATION
{
    float Fps;
    float3 Dummy;
};

cbuffer ParticleGlobalBuffer : register(b7)
{
    PC_INFOMATION PcInfomation;
}
