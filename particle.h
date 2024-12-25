#pragma once

#include "gameObject.h"

//�p�[�e�B�N���̎�����
struct ParticleCompute
{
	XMFLOAT3 Position; //���W
	XMFLOAT3 ShootDirection; //���˕���
	float SpeedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����
	float MaxLife;//�ő����
	float Life; //����
	float Dummy[3]; //�T�C�Y�����p�_�~�[
};

class Particle : public GameObject
{
private:
	//�R���s���[�g�V�F�[�_�\
	ID3D11ComputeShader* m_ComputeShader{};

	//�p�[�e�B�N��
	ParticleCompute* m_Particle{};

	//�o�b�t�@
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};
	ID3D11Buffer* m_PositionBuffer{};
	ID3D11Buffer* m_StagingBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleSRV{};
	ID3D11ShaderResourceView* m_PositionSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ResultUAV{};

	//�p�[�e�B�N���T�C�Y
	XMFLOAT2 m_Size{};

	//�p�[�e�B�N���̍ő吔
	int m_ParticleAmount{};

	//�e�N�X�`�����
	ID3D11ShaderResourceView* m_Texture{};

	float m_SliderValue{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};