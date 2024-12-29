#pragma once

#include "gameObject.h"

//�p�[�e�B�N��1��1�̐ݒ�
struct PARTICLE_LOCAL_CONFIG
{
	XMFLOAT3 Position; //���W
	XMFLOAT3 ShootDirection; //���˕���
	float SpeedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����
	float MaxLife;//�ő����
	float Life; //����
	float Dummy[3]; //�T�C�Y�����p�_�~�[
};

//�p�[�e�B�N���G�t�F�N�g�S�̂̋��ʐݒ�
struct PARTICLE_GLOBAL_CONFIG
{
	float SpeedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����
};


class Particle : public GameObject
{
private:
	//�R���s���[�g�V�F�[�_
	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11ComputeShader* m_PingPongShader{};

	//�W�I���g���V�F�[�_
	ID3D11GeometryShader* m_GeometryShader{};

	//�p�[�e�B�N��
	PARTICLE_LOCAL_CONFIG* m_ParticleLocal{};
	PARTICLE_GLOBAL_CONFIG* m_ParticleGlobal{};

	//�o�b�t�@
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleLocalBuffer{};
	ID3D11Buffer* m_ResultBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleLocalSRV{};
	ID3D11ShaderResourceView* m_ResultSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ParticleLocalUAV{};
	ID3D11UnorderedAccessView* m_ResultUAV{};

	//�p�[�e�B�N���T�C�Y
	XMFLOAT2 m_Size{};

	//�p�[�e�B�N���̍ő吔
	int m_ParticleAmount{};

	//�e�N�X�`�����
	ID3D11ShaderResourceView* m_Texture{};

	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X

	//���C�t
	int m_LifeSlider{};
	//���x
	float m_SpeedSlider{};

	//�p�[�e�B�N���̓��e�ɕύX�����������m�F
	bool m_ChangeParticle{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};