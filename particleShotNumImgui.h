#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleShotNumImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : ���ː�
	int m_ShotNum{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

