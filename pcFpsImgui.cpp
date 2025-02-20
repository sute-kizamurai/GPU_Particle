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

	//DeltaTime��\��
	ImGui::Text("DeltaTime : %.2fms", ImGui::GetIO().DeltaTime * 1000.0f);

	//PC��FPS���Z�b�g
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
