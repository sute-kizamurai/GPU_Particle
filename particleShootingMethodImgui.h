#pragma once

////�C���N���[�h
#include <map>

#include "imguiBase.h"


class ParticleShootingMethodImgui : public ImguiBase
{
private:
	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X : ���˕��@
	std::map<int, XMFLOAT2> m_ShootingMethod{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

