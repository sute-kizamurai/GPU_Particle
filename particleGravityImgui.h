#pragma once

////インクルード
#include "imguiBase.h"


class ParticleGravityImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : 重力の使用フラグ
	bool m_IsEnableGravity{};

	//パーティクルの変更可能ステータス : 重力の強さ
	float m_GravityStrengthSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

