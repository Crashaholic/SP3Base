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
	targetMov = { 0, pos.y, 0 };
	targetPos = ref->pos;
	this->SetPlayerGORef(ref);
	this->SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->type = (Math::RandIntMinMax(0, 1) ? GameObject::ENEMY_TANK_PASSIVE : GameObject::ENEMY_TANK_AGGRESSIVE);
	GOref->pos = pos;
	GOref->scale.Set(4.4f, 1.8f, 1.0f);
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
}

void TankEnemy::Update(double dt)
{
	
}

