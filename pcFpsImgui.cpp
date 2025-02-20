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

	//PCのFPSをセット
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
