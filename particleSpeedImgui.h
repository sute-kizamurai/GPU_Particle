#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleSpeedImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : ���x
	float m_SpeedSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

