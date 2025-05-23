#pragma once

////インクルード
#include <string>

#include "particle.h"
#include "pcInfoImgui.h"


class ImguiBase
{
protected:
	//パーティクルのアドレスを格納
	Particle* m_Target;

	//PC情報用のImGUIのウィンドウアドレスを格納
	PcInfoImgui* m_PcInfoImgui;

	//ImGuiの階層構造に付ける名前
	std::string m_ImguiTreeName;

public:
	ImguiBase() = delete;
	ImguiBase(Particle* Target, std::string TreeName) 
	{
		m_Target = Target;
		m_ImguiTreeName = TreeName;
	}
	ImguiBase(PcInfoImgui* PcInfoImgui, std::string TreeName)
	{
		m_PcInfoImgui = PcInfoImgui;
		m_ImguiTreeName = TreeName;
	}


	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() = 0;

	std::string GetImguiTreeName() { return m_ImguiTreeName; }
};
