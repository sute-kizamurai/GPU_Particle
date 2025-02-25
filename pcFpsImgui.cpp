#include "main.h"
#include "manager.h"
#include "pcFpsImgui.h"

void PcFpsImgui::Init()
{
}

void PcFpsImgui::Draw()
{
	//PCのFPSを表示
	ImGui::Text("FPS : %.2f", ImGui::GetIO().Framerate);

	//DeltaTimeを表示
	ImGui::Text("DeltaTime : %.2fms", ImGui::GetIO().DeltaTime * 1000.0f);


	//DeltaTimeの変化を表すグラフ用の配列を更新
	for (int i = 0; i < 17; i++)
	{
		m_DeltaTimeGraph[i] = m_DeltaTimeGraph[i + 1];
	}
	m_DeltaTimeGraph[17] = ImGui::GetIO().DeltaTime;

	//DeltaTimeのグラフを作成
	ImGui::PlotLines("DeltaTimeGraph", m_DeltaTimeGraph, IM_ARRAYSIZE(m_DeltaTimeGraph));


	//PCのFPSをセット
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
