#pragma once

////インクルード
#include "imguiBase.h"


class ParticleSpeedImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : 速度
	float m_SpeedSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

