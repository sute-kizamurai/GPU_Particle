#include "main.h"
#include "manager.h"
#include "pcFpsImgui.h"

void PcFpsImgui::Init()
{
}

void PcFpsImgui::Draw()
{
	//PC��FPS&�f���^�^�C����\��
	ImGui::Text("%.3f ms", ImGui::GetIO().DeltaTime * 1000.0f);
	ImGui::SameLine();
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

	static float value[180];
	for (int i = 0; i < 179; i++)
		value[i] = value[i + 1];

	value[179] = ImGui::GetIO().DeltaTime * 1000.0f;

	ImGui::PlotLines("GraphDeltaTime", value, sizeof(value) / sizeof(float), 0, NULL, 0.0f, 100.0f, ImVec2(0, 50));
	//PC��FPS��\��
	ImGui::Text("FPS : %.2f", ImGui::GetIO().Framerate);

	//DeltaTime��\��
	ImGui::Text("DeltaTime : %.2fms", ImGui::GetIO().DeltaTime * 1000.0f);


	//DeltaTime�̕ω���\���O���t�p�̔z����X�V
	for (int i = 0; i < 17; i++)
	{
		m_DeltaTimeGraph[i] = m_DeltaTimeGraph[i + 1];
	}
	m_DeltaTimeGraph[17] = ImGui::GetIO().DeltaTime;

	//DeltaTime�̃O���t���쐬
	ImGui::PlotLines("DeltaTimeGraph", m_DeltaTimeGraph, IM_ARRAYSIZE(m_DeltaTimeGraph));


	//PC��FPS���Z�b�g
	m_PcInfoImgui->SetFps(ImGui::GetIO().Framerate);
}
