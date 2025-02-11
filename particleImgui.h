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

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
