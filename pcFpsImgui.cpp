#include "main.h"
#include "manager.h"
#include "pcFpsImgui.h"

void PcFpsImgui::Init()
{
}

void PcFpsImgui::Draw()
{
	//PC��FPS��\��
	ImGui::Text("FPS : %.2f", GetFps());
}
