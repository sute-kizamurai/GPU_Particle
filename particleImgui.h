#pragma once

#include "gameObject.h"
#include "particle.h"

class ParticleImgui : public GameObject
{
private:
	//自身の担当するオブジェクトを格納
	Particle* m_Parent;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};