#pragma once

#include "gameObject.h"


class ParticleGS : public GameObject
{
private:
	//�R���s���[�g�V�F�[�_
	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11ComputeShader* m_PingPongShader{};

	//�W�I���g���V�F�[�_
	ID3D11GeometryShader* m_GeometryShader{};

	//�p�[�e�B�N��
	ParticleCompute* m_Particle{};

	//�o�b�t�@
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};

	//SRV
	ID3D11ShaderResourceView* m_ParticleSRV{};
	ID3D11ShaderResourceView* m_ResultSRV{};

	//UAV
	ID3D11UnorderedAccessView* m_ParticleUAV{};
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