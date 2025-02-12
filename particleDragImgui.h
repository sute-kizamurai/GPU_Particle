#pragma once

////インクルード
#include "imguiBase.h"


class ParticleDragImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : 抵抗力の使用フラグ
	bool m_IsEnableDrag{};

	//パーティクルの変更可能ステータス : 抵抗力の強さ
	float m_DragStrengthSlider{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

