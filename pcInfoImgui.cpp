
////インクルード
#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "pcInfoImgui.h"

void PcInfoImgui::Init()
{

	//ImGuiの要素を初期化
	for (auto element : m_Element)
	{
		element->Init();
	}
}

void PcInfoImgui::Uninit()
{
	//メモリの開放
	for (auto element : m_Element)
	{
		element->Uninit();
		delete element;
	}
}

void PcInfoImgui::Update()
{
}

void PcInfoImgui::Draw()
{
	//ImGui設定
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("PC Infomation");
	for (auto element : m_Element)
	{
		if (ImGui::TreeNode(element->GetImguiTreeName().c_str()))
		{
			element->Draw();
			ImGui::TreePop();
		}
	}

	ImGui::End();
}
