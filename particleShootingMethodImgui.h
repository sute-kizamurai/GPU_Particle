#pragma once

////インクルード
#include <map>

#include "imguiBase.h"


class ParticleShootingMethodImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : 発射方法
	std::map<int, XMFLOAT2> m_ShootingMethod{};

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

