#include "Tank.h"
#include "../../Game/SoundEngine/SoundEngine.h"

Tank::Tank()
{
}

Tank::~Tank()
{
}

void Tank::Init()
{
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("TShoot", "Audio//Tank shoot.wav");
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
		object->type = GameObject::PLAYER_PROJECTILE_SHELL;
		object->scale.Set(0.4f, 0.4f, 0.4f);
		object->pos = GOref->pos;
		object->vel = GOref->dir * 30.0f;
		object->hasGravity = false;
		bulletCooldown = (double)Math::Max(1.5f - (GOManager::GetInstance()->upgrade_1 * 0.5f), 0.5f);
		if (object->active == true)
		{
			CSoundEngine::GetInstance()->PlayASound("TShoot");
		}
	}
}

void Tank::SetGORef(GameObject * GOref)
{
	this->GOref = GOref;
}
