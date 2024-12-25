#pragma once

#include "gameObject.h"

class ComputeShader : public GameObject
{
private:
	//�R���s���[�g�V�F�[�_�\
	ID3D11ComputeShader* m_ComputeShader;

	//�p�[�e�B�N��
	ParticleCompute* m_Particle{};

	//�o�b�t�@
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};
	ID3D11Buffer* m_PositionBuffer{};

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

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};