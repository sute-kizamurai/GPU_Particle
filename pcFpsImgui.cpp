#include "main.h"
#include "manager.h"
#include "pcFpsImgui.h"

void PcFpsImgui::Init()
{
}

void PcFpsImgui::Draw()
{
	//PCのFPS&デルタタイムを表示
	ImGui::Text("%.3f ms", ImGui::GetIO().DeltaTime * 1000.0f);
	ImGui::SameLine();
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

	static float value[180];
	for (int i = 0; i < 179; i++)
		value[i] = value[i + 1];

	value[179] = ImGui::GetIO().DeltaTime * 1000.0f;

	ImGui::PlotLines("GraphDeltaTime", value, sizeof(value) / sizeof(float), 0, NULL, 0.0f, 100.0f, ImVec2(0, 50));

	//PCのFPSをセット
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
