#pragma once

class Component
{
protected:

	class GameObject* m_GameObject = nullptr;

public:
	Component() = delete; //引数なしのコンストラクタを呼べなくする
	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component() {}

	virtual void Init() {} //仮想関数
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};
