
////インクルード
#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleImgui.h"

#include "particleShootingMethodImgui.h"
#include "particleShotNumImgui.h"
#include "particleShotIntervalImgui.h"
#include "particleLifeImgui.h"
#include "particleSpeedImgui.h"
#include "particleGravityImgui.h"
#include "particleColorImgui.h"
#include "particleDragImgui.h"

void ParticleImgui::Init()
{
	//パーティクルのアドレスを格納
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//ImGuiの要素を追加
	m_Element.emplace_back(new ParticleShootingMethodImgui(m_Target, "ShootingMethod"));
	m_Element.emplace_back(new ParticleShotNumImgui(m_Target, "ShotNumber"));
	m_Element.emplace_back(new ParticleShotIntervalImgui(m_Target, "ShotInterval"));
	m_Element.emplace_back(new ParticleLifeImgui(m_Target, "Life(second)"));
	m_Element.emplace_back(new ParticleSpeedImgui(m_Target, "ShootingSpeed"));
	m_Element.emplace_back(new ParticleColorImgui(m_Target, "ParticleColor"));
	m_Element.emplace_back(new ParticleGravityImgui(m_Target, "Gravity"));
	m_Element.emplace_back(new ParticleDragImgui(m_Target, "Drag"));

	//ImGuiの要素を初期化
	for (auto element : m_Element)
	{
		element->Init();
	}
}

void ParticleImgui::Uninit()
{
	//メモリの開放
	for (auto element : m_Element)
	{
		element->Uninit();
		delete element;
	}
}

void ParticleImgui::Update()
{
}

void ParticleImgui::Draw()
{
	//ImGui設定
	ImGui::SetNextWindowPos(ImVec2(1420.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
	ImGui::Begin("ParticleStatus");

	ImGui::Text("Particle count : %d", m_Target->GetParticleAmount());

	for (auto element : m_Element)
	{
		//次の階層構造を最初から開いている状況に設定
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		if(ImGui::TreeNode(element->GetImguiTreeName().c_str()))
		{
			element->Draw();
			ImGui::TreePop();
		}
	}

	ImGui::End();
}
