#include "PlaneEnemy.h"
#include "../../GOManager.h"

#include "Logging.h"
PlaneEnemy::PlaneEnemy()
{
}

PlaneEnemy::PlaneEnemy(vec3 pos, GameObject * ref)
{
	SpawnNewPlaneEnemy(pos, ref, 100);
}


PlaneEnemy::~PlaneEnemy()
{
}


void PlaneEnemy::SpawnNewPlaneEnemy(vec3 pos, GameObject* ref, float m_worldWidth) // ref is Player's GO reference
{
	targetMov = { m_worldWidth - 10, pos.y, 0 };
	targetPos = ref->pos;
	this->SetPlayerGORef(ref);
	this->SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->type = GameObject::ENEMY_PLANE_PASSIVE;
	GOref->pos = pos;
	GOref->scale.Set(4.4f, 1.8f, 1.0f);
	GOref->angle -= Math::DegreeToRadian(0);
	GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f); 
	GOref->norm = GOref->dir;
	GOref->defaultPos = GOref->pos;
	GOref->reserved = true;
	GOref->active = true;
	this->m_worldWidth = m_worldWidth;
	topSpeed = 10;
	priAmmo = 0;
	cooldownLimit = 3;
	cooldown = cooldownLimit;
	originalHeight = pos.y;
	AddPri(10);
}

float GetAngle(vec3 a, vec3 b)
{
	return Math::RadianToDegree(acos(a.Normalized().Dot(b.Normalized())));
}

void PlaneEnemy::Update(double dt)
{
	if (GOref->active)
	{
		if ((GOref->pos - targetMov).Length() > 0.5f)
		{
			vec3 tmpDir = (targetMov - GOref->pos).Normalized();
			GOref->pos += tmpDir * 20.0f * static_cast<float>(dt);
		}
		else
		{
			if (GOref->pos.x > m_worldWidth / 2.0f)
			{
				MoveTo({ 10, originalHeight, 0 });
				GOref->angle = Math::DegreeToRadian(180);
			}
			else
			{
				MoveTo({ m_worldWidth - 10, originalHeight, 0 });
				GOref->angle = Math::DegreeToRadian(0);
			}
		}

		GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
		GOref->vel = GOref->dir * (topSpeed * 0.5f);
		GOref->pos += GOref->vel * (float)dt;
		if (GOref->dir.x < 0)
		{
			GOref->scale.x = -4.4f;
		}
		else
		{
			GOref->scale.x = 4.4f;
		}
	
		cooldown = Math::Max(cooldown - 1.f * (float)dt, 0.f);

		if (abs(GOref->pos.x - playerGO->pos.x) < 30.0f && cooldown <= 0.f)
		{
			Primary();
			cooldown = cooldownLimit;
		}

		for (unsigned int i = 0; i < priProjectiles.size(); ++i)
		{
			if (priProjectiles[i])
			{
				if (!priProjectiles[i]->active || priProjectiles[i]->type != GameObject::PLAYER_PROJECTILE_BOMB)
				{
					priProjectiles[i] = NULL;
					//break;
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
			GameObject* bomb = GOManager::GetInstance()->fetchGO();
			bomb->type = GameObject::ENEMY_PROJECTILE_BOMB;
			bomb->vel = { GOref->vel.x, GOref->vel.y, GOref->vel.z };
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

