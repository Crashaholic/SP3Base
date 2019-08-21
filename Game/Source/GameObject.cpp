
#include "GameObject.h"
#include "GOManager.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	reserved(false),
	exRadius(1.f),
	hasGravity(false),
	norm(0, 1, 0),
	angle(0.0f),
	defaultPos(0.0f, 0.0f, 0.0f),
	defaultScale(1.0f, 1.0f, 1.0f),
	wrapMode(SW_CLEAR),
	lifeTime(0.0),
	hasLifeTime(false),
	transparency(1.0f),
	Iframes(0.0)
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

void GameObject::Init()
{
}

void GameObject::Update(double dt)
{
	if (Iframes > 0)
		Iframes -= dt;
	else
		Iframes = 0;
	pos += vel * (float)dt;
	if (hasLifeTime)
	{
		lifeTime -= dt;
		if (lifeTime <= 0.0)
		{
			active = false;
			hasLifeTime = false;
			lifeTime = 0;
			return;
		}
	}
	switch (type)
	{
	case PLAYER_PROJECTILE_MACHINE:
		try
		{
			dir = vel.Normalized();
		}
		catch (DivideByZero)
		{
			dir.Set(1, 0, 0);
		}
		angle = atan2(dir.y, dir.x);
		norm.Set(-dir.y, dir.x);
		break;
	case PLAYER_PROJECTILE_MISSILE:
		vel += dir * static_cast<float>(dt)*100.0f;
		vel += GOManager::GetInstance()->gravity * static_cast<float>(dt)*vel.Length() * 0.1f;
		try
		{
			norm = Vector3(-vel.y, vel.x).Normalized();
		}
		catch (DivideByZero)
		{
			norm.Set(1, 0, 0);
		}
		angle = atan2(dir.y, dir.x);
		//norm.Set(-dir.y, dir.x);
		break;

	case PLAYER_PROJECTILE_NUKE:
	case PLAYER_PROJECTILE_BOMB:
	case ENEMY_PROJECTILE_BOMB:
		try
		{
			dir = vel.Normalized();
		}
		catch (DivideByZero)
		{
			dir.Set(1, 0, 0);
		}
		angle = atan2(dir.y, dir.x);
		norm.Set(-dir.y, dir.x);
		break;
	case EXPLOSION:
		transparency = (float)lifeTime;
		if (lifeTime > 0.0)
		{
			scale += defaultScale * 0.7f * static_cast<float>(dt);
		}
		break;
	}
}

bool GameObject::hasCollider()
{
	switch (type)
	{
	case GO_NONE:
	case EXPLOSION:
	case GO_RAIN:
	case GO_TOTAL:
		return false;
	}
	return true;
}

void GameObject::reset()
{
	angle = 0.0f;
	dir.Set(cos(angle), sin(angle), 0.0f);
	pos = defaultPos;
	vel.SetZero();
}