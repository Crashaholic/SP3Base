
#include "GameObject.h"

bool GameObject::hasCollider()
{
	switch (type)
	{
	case GO_NONE:
	case EXPLOSION:
	case GO_TOTAL:
		return false;
	}
	return true;
}

void GameObject::Update(double dt)
{
	pos += vel * (float)dt;
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	hasGravity(false),
	norm(0, 1, 0),
	angle(0.0f),
	wrapMode(SW_CLEAR)
{
}

GameObject::~GameObject()
{
}

