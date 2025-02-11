#pragma once

////インクルード
#include <vector>

#include "imguiBase.h"
#include "component.h"

////前方宣言
class Particle;

class ParticleImgui : public Component
{
private:
	//自身の担当するオブジェクトを格納
	Particle* m_Target;

	//パーティクル用のImGuiを格納する変数
	std::vector<ImguiBase*> m_Element;

	//パーティクルの変更可能ステータス
	//ライフ
	int m_LifeSlider{};
	//速度
	float m_SpeedSlider{};
	//重力の使用フラグ
	bool m_IsEnableGravity{};
	//重力の強さ
	float m_GravityStrengthSlider{};


public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//変更可能ステータスの初期値を設定
	void SetModifiableStatus();
};