#pragma once

////�C���N���[�h
#include "imguiBase.h"


class ParticleColorImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : �F
	XMFLOAT4 m_ParticleColor{};

	//ImGui�J���[�s�b�J�[����擾�ł���l���ꎞ�I�Ɋi�[
	float m_Buf[4];

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

