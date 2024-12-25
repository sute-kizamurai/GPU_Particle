#pragma once

#include "gameObject.h"

class Polygon2DTitle : public GameObject
{
private:
	
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	class Audio* m_BGM{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
