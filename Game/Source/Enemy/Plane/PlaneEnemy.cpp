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
	this->SetGORef(GOManager::GetInstance()->fetchGO());
	this->SetPlayerGORef(ref);
	GOref->type = GameObject::ENEMY_PLANE_PASSIVE;
	targetMov = { m_worldWidth - 10, 50, 0 };
	GOref->pos = pos;
	targetPos = ref->pos;
	GOref->scale.Set(4.4f, 1.8f, 1.0f);
	GOref->angle -= Math::DegreeToRadian(0);
	GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f); 
	GOref->norm = GOref->dir;
	GOref->defaultPos = GOref->pos;
	GOref->active = true;
	this->m_worldWidth = m_worldWidth;
	topSpeed = 10;
	priAmmo = 0;
	AddPri(10);
}

float GetAngle(vec3 a, vec3 b)
{
	return Math::RadianToDegree(acos(a.Normalized().Dot(b.Normalized())));
}

void PlaneEnemy::Update(double dt)
{
	if ((GOref->pos - targetMov).Length() > 0.5f)
	{
		vec3 tmpDir = (targetMov - GOref->pos).Normalized();
		GOref->pos += tmpDir * 20 * dt;
	}
	else
	{
		if (GOref->pos.x > m_worldWidth / 2.0f)
		{
			MoveTo({ 10, 50, 0 });
			GOref->angle = Math::DegreeToRadian(180);
		}
		else
		{
			MoveTo({ m_worldWidth - 10, 50, 0 });
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
	if (abs(GOref->pos.x - playerGO->pos.x) < 20.0f /*&&*/ )
		Primary();

	for (unsigned int i = 0; i < priprojectiles.size(); ++i)
	{
		if (priprojectiles[i])
		{
			if (!priprojectiles[i]->active || priprojectiles[i]->type != GameObject::PLAYER_PROJECTILE_BOMB)
			{
				priprojectiles[i] = NULL;
				//break;
			}
		}
	}
}

void PlaneEnemy::Primary()
{
	for (unsigned int i = 0; i < priprojectiles.size(); ++i)
	{
		if (!priprojectiles[i])
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
			priprojectiles[i] = bomb;
			++GOManager::GetInstance()->totalShots;
			break;
		}
	}
}

