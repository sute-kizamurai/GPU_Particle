#pragma once

////インクルード
#include <vector>

#include "gameObject.h"

////構造体宣言
struct PC_INFOMATION
{
	float Fps;
	XMFLOAT3 Dummy;
};


////前方宣言
class ImguiBase;

class PcInfoImgui : public GameObject
{
private:
	//パーティクル用のImGuiを格納する変数
	std::vector<ImguiBase*> m_Element;

	//コンピュートシェーダにPCの基本情報を送信するための構造体
	PC_INFOMATION* m_PcInfomation;

	//コンピュートシェーダにPCの基本情報を送信するためのバッファ
	ID3D11Buffer* m_PcInfoBuffer;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//セッター＆ゲッター
	void SetFps(float Fps) { m_PcInfomation->Fps = Fps; }
};

