#pragma once

////インクルード
#include "imguiBase.h"


class PcFpsImgui : public ImguiBase
{
private:
	float m_DeltaTimeGraph[18]{};


public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

