#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleShotIntervalImgui : public ImguiBase
{
private:
	//�ύX�\�X�e�[�^�X : ���ˋ���(���ˉ\��1�A���˕s�\��0)
	int m_Fireable{};

	//�O�񔭎˂���̌o�ߎ��Ԃ������
	float m_ElapsedTime{};

	//���˃C���^�[�o���������
	float m_ShotInterval{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

