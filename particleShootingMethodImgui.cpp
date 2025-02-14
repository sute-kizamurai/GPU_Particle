#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleShootingMethodImgui.h"

void ParticleShootingMethodImgui::Init()
{
	//発射方法のデフォルト値を格納
	m_ShootingMethod[0] = { 2.0f, 1.0f };
	m_ShootingMethod[1] = { 0.0f, 0.0f };
	m_ShootingMethod[2] = { 0.5f, -0.5f };

}

void ParticleShootingMethodImgui::Draw()
{
	//発射方法の文字をまとめた配列
	const char* items[] = { "Sphere", "Hemisphere", "Fountain" };

	static int item_current = 0;

	//パーティクルの発射方法を変更できるコンボを表示
	if (ImGui::Combo("ShootingMethod", &item_current, items, IM_ARRAYSIZE(items)))
	{
	}
}
