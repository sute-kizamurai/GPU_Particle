#pragma once

////インクルード
#include <vector>

#include "imguiBase.h"
#include "gameObject.h"

class PcInfoImgui : public GameObject
{
private:
	//パーティクル用のImGuiを格納する変数
	std::vector<ImguiBase*> m_Element;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

