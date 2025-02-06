#pragma once

#include "component.h"
#include "particle.h"

class ParticleImgui : public Component
{
private:
	//自身の担当するオブジェクトを格納
	Particle* m_Target;

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};