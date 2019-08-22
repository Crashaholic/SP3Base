#include "zcustompch.h"
#include "Tank.h"

Tank::Tank()
{
}

Tank::~Tank()
{
}

void Tank::Init()
{
}

void Tank::Update(double dt)
{
	int i = 0;
}

void Tank::Move_LeftRight(double dt, bool left)
{
	if (left)
		this->GOref->pos.x -= tankSpeed * static_cast<float>(dt) * GOref->norm.x;
	else
		this->GOref->pos.x += tankSpeed * static_cast<float>(dt) * GOref->norm.x;
}


void Tank::Fire()
{
	if (bulletCooldown <= 0.0)
	{
		GameObject *object = GOManager::GetInstance()->fetchGO();
		object->active = true;
		object->wrapMode = GameObject::SW_CLEAR;
		object->type = GameObject::PLAYER_PROJECTILE_SHELL;
		object->scale.Set(0.4f, 0.4f, 0.4f);
		object->pos = GOref->pos;
		object->vel = GOref->dir * 60.0f;
		object->hasGravity = false;
		bulletCooldown = (double)Math::Max(2.0f - (GOManager::GetInstance()->upgrade_1 * 0.2f), 0.6f);
		GOManager::GetInstance()->playSound("TShoot");
	}
}

void Tank::SetGORef(GameObject * GOref)
{
	this->GOref = GOref;
}
