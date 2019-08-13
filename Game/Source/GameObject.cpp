
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f)
{
}

GameObject::~GameObject()
{
}

inline void GameObject::SetPos(const Vector3 & pos)
{
	this->pos = pos;
}

inline Vector3 GameObject::GetPos()
{
	return pos;
}
