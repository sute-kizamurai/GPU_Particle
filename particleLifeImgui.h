#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleLifeImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : ���C�t
	float m_LifeSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

