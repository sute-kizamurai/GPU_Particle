#pragma once

////�C���N���[�h
#include "gameObject.h"


////�\���̐錾
//�p�[�e�B�N���ʂ̐ݒ�
struct PARTICLE_LOCAL_CONFIG
{
	XMFLOAT4 Position; //���W
	XMFLOAT3 ShootDirection; //���˕���
	float Life; //��������
};

//�p�[�e�B�N���G�t�F�N�g�S�̂̋��ʐݒ�iGPU���œǂݍ��ݐ�p�j
struct PARTICLE_GLOBAL_CONFIG_R
{
	XMFLOAT2 ShootingMethod; //���˕��������肷�邽�߂̕␳�l���i�[

	int Fireable; //�p�[�e�B�N�������ˉ\���ǂ����𑗂�(���ˉ\��1�A���˕s�\��0)

	float MaxLife; //�p�[�e�B�N���̍ő����

	float SpeedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����

	BOOL IsEnableGravity; //�d�͂��g�p���邩�ǂ����̃t���O
	float GravityFactor; //�d�͂̋���

	BOOL IsEnableDrag; //��R�͂��g�p���邩�ǂ����̃t���O
	float DragFactor; //��R�͂̋���
	
	float DummyFloat; //�T�C�Y�����p�_�~�[(float�^)
	BOOL DummyBool[2]; //�T�C�Y�����p�_�~�[(bool�^)
};

//�p�[�e�B�N���G�t�F�N�g�S�̂̋��ʐݒ�iGPU���œǂݏ����\�j
struct PARTICLE_GLOBAL_CONFIG_RW
{
	int ShotNum; //��x�ɔ��˂ł���p�[�e�B�N���̐�
	float DummyFloat[3]; //�T�C�Y�����p�_�~�[(float�^)
};


class Particle : public GameObject
{
private:
	//�R���s���[�g�V�F�[�_
	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11ComputeShader* m_ParticleInitialShader{};
	ID3D11ComputeShader* m_PingPongShader{};

	//�W�I���g���V�F�[�_
	ID3D11GeometryShader* m_GeometryShader{};

	//�p�[�e�B�N��
	PARTICLE_LOCAL_CONFIG* m_ParticleLocal{};
	PARTICLE_GLOBAL_CONFIG_R* m_ParticleGlobalRead{};
	PARTICLE_GLOBAL_CONFIG_RW* m_ParticleGlobalReadWrite{};

	//�o�b�t�@
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleLocalBuffer{};
	ID3D11Buffer* m_ParticleGlobalReadBuffer{}; //GPU���œǂݍ��ݐ�p�̃o�b�t�@
	ID3D11Buffer* m_ParticleGlobalReadWriteBuffer{}; //GPU���œǂݏ����\�ȃo�b�t�@
	ID3D11Buffer* m_ResultBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleLocalSRV{};
	ID3D11ShaderResourceView* m_ResultSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ParticleLocalUAV{};
	ID3D11UnorderedAccessView* m_ResultUAV{};
	ID3D11UnorderedAccessView* m_m_ParticleGlobalReadWriteUAV{};

	//�p�[�e�B�N���T�C�Y
	XMFLOAT2 m_Size{};

	//�p�[�e�B�N���̍ő吔
	int m_ParticleAmount{};

	//�e�N�X�`�����
	ID3D11ShaderResourceView* m_Texture{};

	//�p�[�e�B�N���̐F���
	XMFLOAT4 m_ParticleColor{};

	//�p�[�e�B�N���̓��e�ɕύX�����������m�F
	bool m_ChangeParticle{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//�p�[�e�B�N���̌ʐݒ�𐶐�����
	//IN ParticleAmount : �p�[�e�B�N���̔��ː��𐶐�
	void CreateParticleMaxCapacity(int ParticleAmount);

	//�p�[�e�B�N���̑S�̐ݒ�𐶐�����
	void CreateParticleGlobal();

public://�Z�b�^�[���Q�b�^�[
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