#pragma once

#include <list>
#include <vector>

#include "gameObject.h"
#include "camera.h"
#include "particleEmitter.h"
#include "polygon2DTitle.h"
#include "particle.h"
#include "particleGS.h"

#define LayerNum (4)

enum class LayerName
{
	System = 0,
	Object,
	Effect,
	Ui,
};


class Scene
{
private:
	std::list<GameObject*> m_GameObject[LayerNum];

public:
	virtual ~Scene() {};

	void Init() {
		AddGameObject<Camera>((int)LayerName::System);
		//AddGameObject<ParticleEmitter>((int)LayerName::Object);
		AddGameObject<Particle>((int)LayerName::Object);
		//AddGameObject<ParticleGS>((int)LayerName::Object);
		//AddGameObject<Polygon2DTitle>((int)LayerName::Ui);
	}

	template <typename T> //テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T> //テンプレート関数
	T* GetGameObject()
	{
		for (int i = 0; i < LayerNum; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(object);

				if (ret != nullptr)
					return ret;
			}
		}

		return nullptr;
	}

	template <typename T> //テンプレート関数
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objectList;

		for (int i = 0; i < LayerNum; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(object);

				if (ret != nullptr)
					objectList.push_back(ret);
			}
		}

		return objectList;
	}

	void Uninit()
	{
		for (int i = 0; i < LayerNum; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}

			m_GameObject[i].clear();
		}
	}

	void Update()
	{
		for (int i = 0; i < LayerNum; i++)
		{
			for (GameObject* object : m_GameObject[i])//範囲for
			{
				object->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) { return object->Destroy(); }); //ラムダ式
		}
	}

	void Draw()
	{
		for (int i = 0; i < LayerNum; i++)
		{
			//Zソート
			//m_GameObject[i].sort()

			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}
};