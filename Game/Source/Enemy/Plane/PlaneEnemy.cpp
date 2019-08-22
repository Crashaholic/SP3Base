#include "zcustompch.h"
#include "PlaneEnemy.h"
#include "../../GOManager.h"

#include "Logging.h"


PlaneEnemy::PlaneEnemy()
{
}

PlaneEnemy::PlaneEnemy(vec3 pos, GameObject * ref, float m_worldWidth)
{
	SpawnNewPlaneEnemy(pos, ref, m_worldWidth);
}


PlaneEnemy::~PlaneEnemy()
{
}


void PlaneEnemy::SpawnNewPlaneEnemy(vec3 pos, GameObject* ref, float m_worldWidth) // ref is Player's GO reference
{
	targetMov = {0, pos.y, 0 };
	targetPos = ref->pos;
	this->SetPlayerGORef(ref);
	this->SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->type = (Math::RandIntMinMax(0, 1) ? GameObject::ENEMY_PLANE_PASSIVE : GameObject::ENEMY_PLANE_AGGRESSIVE);
	GOref->pos = pos;
	switch (GOref->type)
	{
	case GameObject::ENEMY_PLANE_PASSIVE:
		GOref->scale = Vector3(6.0f, 1.9f, 1.0f)* 3.0f;
		break;
	case GameObject::ENEMY_PLANE_AGGRESSIVE:
		GOref->scale.Set(4.4f, 1.8f, 1.0f);
		break;
	}
	GOref->angle -= Math::DegreeToRadian(0);
	GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f); 
	GOref->norm = GOref->dir;
	GOref->defaultPos = GOref->pos;
	GOref->wrapMode = GameObject::SW_OFFSCREENCLEAR;
	GOref->reserved = true;
	GOref->active = true;
	GOref->hasGravity = false;
	GOref->vel.Set(1, 0, 0);
	GOref->color[0].Set(0.6f, 0.4f, 0.2f);
	GOref->color[1].Set(0.5f, 0.3f, 0.1f);
	this->m_worldWidth = m_worldWidth;
	topSpeed = 20;
	priAmmo = 0;
	cooldownLimit = 0.3f;
	cooldown = cooldownLimit;
	originalHeight = pos.y;
	AddPri(2);
	//priProjectiles = { NULL };
}

float GetAngle(vec3 a, vec3 b)
{
	return Math::RadianToDegree(acos(a.Normalized().Dot(b.Normalized())));
}

void PlaneEnemy::Update(double dt)
{
	if (GOref->active)
	{
		if (GOref->pos.x > m_worldWidth - 5)
		{
			directionleft = true;
			GOref->pos.x = m_worldWidth - 5;
			//GOref->vel.x *= -1;
			targetMov.Set(0, originalHeight + Math::RandFloatMinMax((float)HEIGHT_RANGE,(float)HEIGHT_RANGE), GOref->pos.z);
		}
		else if (GOref->pos.x < 11)
		{
			directionleft = false;
			GOref->pos.x = 11;
			//GOref->vel.x *= -1;
			targetMov.Set(m_worldWidth, originalHeight + Math::RandFloatMinMax(- (float)HEIGHT_RANGE,(float)HEIGHT_RANGE), GOref->pos.z);
		}
		GOref->dir = (targetMov - GOref->pos).Normalized();
		//GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
		GOref->norm = GOref->dir;
		GOref->vel = GOref->dir * (topSpeed * 0.5f);
		GOref->pos += GOref->vel * (float)dt;

		if (GOref->dir.x < 0)
		{
			if(GOref->scale.y>0)
				GOref->scale.y = -GOref->scale.y;
		}
		else
		{
			if (GOref->scale.y <0)
				GOref->scale.y = -GOref->scale.y;
		}

		if (GOref->type == GameObject::ENEMY_PLANE_AGGRESSIVE)
		{
			cooldown = Math::Max(cooldown - 1.f * (float)dt, 0.f);
			Vector3 windSpeed = GOManager::GetInstance()->windVector;
			float heightDifference = GOref->pos.y - playerGO->pos.y;
			double timeToHit = fabs(Math::Max(-sqrt(2 * abs(GOManager::GetInstance()->gravity.y + windSpeed.y)*heightDifference), sqrt(2 * abs(GOManager::GetInstance()->gravity.y + windSpeed.y)*heightDifference)) / GOManager::GetInstance()->gravity.y);
			if (abs(GOref->pos.x - playerGO->pos.x) < abs(GOref->vel.x) * (float)timeToHit - windSpeed.x * (float)timeToHit  && cooldown <= 0.f)
			{
				Primary();
				cooldown = cooldownLimit;
			}

			for (unsigned int i = 0; i < priProjectiles.size(); ++i)
			{
				if (priProjectiles[i])
				{
					if (!priProjectiles[i]->active || priProjectiles[i]->type != GameObject::ENEMY_PROJECTILE_BOMB)
					{
						priProjectiles[i] = NULL;
						//break;
					}
				}
			}
		}
	}
}

void PlaneEnemy::Primary()
{
	for (unsigned int i = 0; i < priProjectiles.size(); ++i)
	{
		if (!priProjectiles[i])
		{
			GOManager::GetInstance()->playSound("PBomb");
			GameObject* bomb = GOManager::GetInstance()->fetchGO();
			bomb->type = GameObject::ENEMY_PROJECTILE_BOMB;
			bomb->vel = GOref->vel;
			bomb->pos = GOref->pos;
			bomb->scale.Set(1, 2, 1);
			bomb->hasGravity = true;
			bomb->wrapMode = GameObject::SW_CLEAR;
			for (int i = 0; i < MAX_TEXTURES; ++i)
				bomb->color[i] = GOref->color[i];
			priProjectiles[i] = bomb;
			++GOManager::GetInstance()->totalShots;
			break;
		}
	}
}

