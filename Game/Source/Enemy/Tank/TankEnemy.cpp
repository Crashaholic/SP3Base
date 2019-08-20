#include "TankEnemy.h"

TankEnemy::TankEnemy()
{

}

TankEnemy::TankEnemy(vec3 pos, GameObject* ref, float m_worldWidth)
{
	SpawnNewTankEnemy(pos, ref, m_worldWidth);
}

TankEnemy::~TankEnemy()
{
}

void TankEnemy::SpawnNewTankEnemy(vec3 pos, GameObject * ref, float m_worldWidth)
{
	targetMov = { m_worldWidth / 2.f, 0, 0 };
	targetPos = ref->pos;
	this->SetPlayerGORef(ref);
	this->SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->type = (Math::RandIntMinMax(0, 1) ? GameObject::ENEMY_TANK_PASSIVE : GameObject::ENEMY_TANK_AGGRESSIVE);
	GOref->pos = pos;
	GOref->scale.Set(4.0f, 2.2f, 1.0f);
	GOref->angle -= Math::DegreeToRadian(0);
	GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
	GOref->norm = GOref->dir;
	GOref->defaultPos = GOref->pos;
	GOref->wrapMode = GameObject::SW_OFFSCREENCLEAR;
	GOref->reserved = true;
	GOref->active = true;
	GOref->hasGravity = false;
	GOref->vel.Set(1, 0, 0);
	this->m_worldWidth = m_worldWidth;
	Stage = 1;
	tankSpeed = 20.f;
}

void TankEnemy::Move_LeftRight(double dt, bool left)
{
	if (left)
		this->GOref->pos.x -= tankSpeed * static_cast<float>(dt) * GOref->norm.x;
	else
		this->GOref->pos.x += tankSpeed * static_cast<float>(dt) * GOref->norm.x;
	Terrain* terreference = GOManager::GetInstance()->terreference;
	Vector3 frontCheck = GOref->pos + Vector3(GOref->scale.x / 2, 0, 0);
	Vector3 rearCheck = GOref->pos - Vector3(GOref->scale.x / 2, 0, 0);
	GOref->angle = atan2(terreference->GetHeight(frontCheck).y - terreference->GetHeight(rearCheck).y, GOref->scale.x);
	GOref->norm.Set(cos(GOref->angle), sin(GOref->angle), 0);
	GOref->dir.Set(-GOref->norm.y, GOref->norm.x);
	GOref->pos.y = (terreference->GetHeight(frontCheck).y + terreference->GetHeight(rearCheck).y) / 2 + heightOffset;
	GOref->pos.x = Math::Clamp(GOref->pos.x, 4.f, 173.f);
}

void TankEnemy::Update(double dt)
{
	switch (Stage)
	{
	case 1:
	{
		if (GOref->pos.x < targetMov.x)
			Move_LeftRight(dt, 0);
		else if (GOref->pos.x > targetMov.x)
			Move_LeftRight(dt, 1);
		else
			Stage = 2;
		break;
	}
	case 2:
	{
		FireAt(playerGO->pos);
		MoveTo(playerGO->pos);
		if (GOref->pos.x - targetMov.x > Range)
			Move_LeftRight(dt, 0);
		else if (GOref->pos.x - targetMov.x < Range)
			Move_LeftRight(dt, 1);
		else
		{
			GOref->dir = playerGO->pos - GOref->pos;
			Fire();
		}
		break;
	}
	}
}

