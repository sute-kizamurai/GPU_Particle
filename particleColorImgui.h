#pragma once

////インクルード
#include "imguiBase.h"


class ParticleColorImgui : public ImguiBase
{
private:
	//パーティクルの変更可能ステータス : 色
	XMFLOAT4 m_ParticleColor{};

	//ImGuiカラーピッカーから取得できる値を一時的に格納
	float m_Buf[4];

public:
	using ImguiBase::ImguiBase;

	void Init() override;
	void Draw() override;
};

