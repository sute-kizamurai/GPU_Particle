#pragma once

////インクルード
#include <vector>

#include "imguiBase.h"
#include "gameObject.h"

////構造体宣言
struct PC_INFOMATION
{
	float Fps;

};

class PcInfoImgui : public GameObject
{
private:
	//パーティクル用のImGuiを格納する変数
	std::vector<ImguiBase*> m_Element;

	//コンピュートシェーダにPCの基本情報を送信するための構造体
	PC_INFOMATION* m_PcInfomation;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

