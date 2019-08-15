
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
	switch (type)
	{
	case PLAYER_PROJECTILE_BOMB:
	case PLAYER_PROJECTILE_NUKE:
		try
		{
			dir = vel.Normalized();
		}
		catch (DivideByZero)
		{
			dir.Set(1, 0, 0);
		}
		angle = atan2(dir.y, dir.x);
		norm.Set( -dir.y, dir.x);
		break;
	}
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
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		isColorable[i] = false;
		color[i].Set(1, 1, 1);
	}
}

GameObject::~GameObject()
{
}

