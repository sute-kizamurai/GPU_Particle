#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleGravityImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : �d�͂̎g�p�t���O
	bool m_IsEnableGravity{};

	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : �d�͂̋���
	float m_GravityStrengthSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

