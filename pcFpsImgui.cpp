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

	//PCのFPSをセット
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
