#pragma once

////インクルード
#include "imguiBase.h"


class ParticleShotNumImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : 発射数
	int m_ShotNum{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

