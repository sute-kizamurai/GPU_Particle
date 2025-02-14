#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleShootingMethodImgui.h"

void ParticleShootingMethodImgui::Init()
{
	//���˕��@�̃f�t�H���g�l���i�[
	m_ShootingMethod[0] = { 2.0f, 1.0f };
	m_ShootingMethod[1] = { 0.0f, 0.0f };
	m_ShootingMethod[2] = { 0.5f, -0.5f };

}

void ParticleShootingMethodImgui::Draw()
{
	//���˕��@�̕������܂Ƃ߂��z��
	const char* items[] = { "Sphere", "Hemisphere", "Fountain" };

	static int item_current = 0;

	//�p�[�e�B�N���̔��˕��@��ύX�ł���R���{��\��
	if (ImGui::Combo("ShootingMethod", &item_current, items, IM_ARRAYSIZE(items)))
	{
	}
}
