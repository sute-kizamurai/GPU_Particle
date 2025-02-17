#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"


//Staticƒƒ“ƒo•Ï”‚ÍéŒ¾‚ª•K—v
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();

	m_Scene = new Scene();
	m_Scene->Init();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;


	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();

	if (m_NextScene != nullptr)
	{
		m_Scene->Uninit();
		delete m_Scene;

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}
}

