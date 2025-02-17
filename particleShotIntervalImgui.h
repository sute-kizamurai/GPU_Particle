#pragma once

////インクルード
#include "imguiBase.h"


class ParticleShotIntervalImgui : public ImguiBase
{
private:
	//変更可能ステータス : 発射許可(発射可能は1、発射不可能は0)
	int m_Fireable{};

	//前回発射からの経過時間をいれる
	float m_ElapsedTime{};

	//発射インターバルをいれる
	float m_ShotInterval{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

