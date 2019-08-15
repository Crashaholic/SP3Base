
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	hasGravity(false),
	norm(0, 1, 0),
	angle(0.0f),
	defaultPos(0.0f, 0.0f, 0.0f),
	wrapMode(SW_CLEAR)
{
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		isColorable[i] = false;
		color[i].Set(1, 1, 1);
	}
}

GameObject::~GameObject()
{
}

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

void GameObject::reset()
{
	angle = 0.0f;
	dir.Set(cos(angle), sin(angle), 0.0f);
	pos = defaultPos;
	vel.SetZero();
}