#include "main.h"
#include "manager.h"
#include "pcFpsImgui.h"

void PcFpsImgui::Init()
{
}

void PcFpsImgui::Draw()
{
	//PC��FPS��\��
	ImGui::Text("FPS : %.2f", ImGui::GetIO().Framerate);

	//PC��FPS���Z�b�g
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
