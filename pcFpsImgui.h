#pragma once

////�C���N���[�h
#include "imguiBase.h"


class PcFpsImgui : public ImguiBase
{
private:

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

