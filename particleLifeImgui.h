#pragma once

////インクルード
#include "imguiBase.h"


class ParticleLifeImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : ライフ
	float m_LifeSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

