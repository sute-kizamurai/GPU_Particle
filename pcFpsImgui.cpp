#include "main.h"
#include "manager.h"
#include "pcFpsImgui.h"

void PcFpsImgui::Init()
{
}

void PcFpsImgui::Draw()
{
	//PCのFPSを表示
	ImGui::Text("FPS : %.2f", GetFps());

	//PCのFPSをセット
	m_PcInfoImgui->SetFps(GetFps());
}
