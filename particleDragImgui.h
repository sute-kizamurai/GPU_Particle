#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleDragImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : ��R�͂̎g�p�t���O
	bool m_IsEnableDrag{};

	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : ��R�͂̋���
	float m_DragStrengthSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

