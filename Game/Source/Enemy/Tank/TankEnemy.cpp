#include "zcustompch.h"
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
	targetMov = { m_worldWidth, 0, 0 };
	targetPos = ref->pos;
	this->SetPlayerGORef(ref);
	this->GOref = (GOManager::GetInstance()->fetchReservedGO());
	GOref->type = (Math::RandIntMinMax(0, 1) ? GameObject::ENEMY_TANK_PASSIVE : GameObject::ENEMY_TANK_AGGRESSIVE);
	GOref->pos = pos;
	if (GOref->type == GameObject::ENEMY_TANK_PASSIVE)
	{
		GOref->scale.Set(3.9f, 1.8f, 1.0f);

	}
	else
	{
		GOref->scale.Set(4.0f, 2.2f, 1.0f);

	}
	GOref->angle -= Math::DegreeToRadian(0);
	GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
	GOref->norm = GOref->dir;
	GOref->defaultPos = GOref->pos;
	GOref->wrapMode = GameObject::SW_OFFSCREENCLEAR;
	GOref->reserved = true;
	GOref->active = true;
	GOref->hasGravity = false;
	GOref->vel.Set(0, 0, 0);
	GOref->color->Set(241.0f / 255.0f, 227.0f / 255.0f, 204.0f / 255.0f);
	heightOffset = GOref->scale.y + 0.5f;
	GOref->scale *= 1.5f;
	this->m_worldWidth = m_worldWidth;
	Stage = 1;
	tankSpeed = 20.f;
}

void TankEnemy::Move_LeftRight(double dt, bool left)
{
	if (left)
	{
		this->GOref->pos.x -= tankSpeed * static_cast<float>(dt) * GOref->norm.x;
		//if (GOref->scale.x > 0)
		//	GOref->scale.x = -GOref->scale.x;
	}
	else
	{
		this->GOref->pos.x += tankSpeed * static_cast<float>(dt) * GOref->norm.x;
		//if (GOref->scale.x < 0)
			//GOref->scale.x = -GOref->scale.x;
	}

	Terrain* terreference = GOManager::GetInstance()->terreference;
	Vector3 frontCheck = GOref->pos + Vector3(fabs(GOref->scale.x) / 2, 0, 0);
	Vector3 rearCheck = GOref->pos - Vector3(fabs(GOref->scale.x) / 2, 0, 0);
	GOref->angle = atan2(terreference->GetHeight(frontCheck).y - terreference->GetHeight(rearCheck).y, fabs(GOref->scale.x));
	GOref->norm.Set(fabs(cos(GOref->angle)), sin(GOref->angle), 0);
	//GOref->dir.Set(-GOref->norm.y, GOref->norm.x);
	GOref->pos.y = (terreference->GetHeight(frontCheck).y + terreference->GetHeight(rearCheck).y) / 2 + heightOffset;
	//GOref->pos.x = Math::Clamp(GOref->pos.x, 4.f, 173.f);
}
void TankEnemy::Fire()
{
	if (bulletCooldown <= 0.0)
	{
		GOManager::GetInstance()->playSound("TShoot");
		GameObject *object = GOManager::GetInstance()->fetchGO();
		object->active = true;
		object->wrapMode = GameObject::SW_CLEAR;
		object->type = GameObject::ENEMY_PROJECTILE_MACHINE;
		object->scale.Set(0.4f, 0.4f, 0.4f);
		object->pos = GOref->pos;
		object->vel = GOref->dir * 60.0f;
		object->hasGravity = false;
		bulletCooldown = (double)Math::RandFloatMinMax(1.0f, 5.0f);
	}
}

void TankEnemy::Update(double dt)
{
	if (GOref->active)
	{
		float shootrange = 10.0f;
		float leftTarget = GOref->scale.x;
		float rightTarget = m_worldWidth - GOref->scale.x;
		bulletCooldown -= dt;
		FireAt(playerGO->pos);
		//MoveTo(playerGO->pos);
		try
		{
			GOref->dir = (playerGO->pos - GOref->pos).Normalized();
		}
		catch (DivideByZero)
		{
			GOref->dir.Set(1, 0, 0);
		}
		if (GOref->type == GameObject::ENEMY_TANK_AGGRESSIVE)
		Fire();
		
			switch (Stage)
			{
			case 1:
			{
				if (GOref->pos.x < targetMov.x)
				{
					Move_LeftRight(dt, 0);
				}
				else
				{
					GOref->pos.x = targetMov.x;
					GOref->angle += Math::DegreeToRadian(180.0f);
					targetMov.x = leftTarget;
					Stage = 2;
					//GOref->scale.x *= -1;
				}
			}
				break;
			case 2:
			{
				if (GOref->pos.x > targetMov.x)
				{
					Move_LeftRight(dt, 1);
				}
				else
				{
					GOref->pos.x = targetMov.x;
					GOref->angle += Math::DegreeToRadian(180.0f);
					targetMov.x = rightTarget;
					Stage = 1;
					//GOref->scale.x *= -1;
				}
				//if (GOref->pos.x - targetMov.x > (float)Range)
				//{
				//	Move_LeftRight(dt, 1);
				//	targetMov.x = leftTarget;
				//}
				//else if (GOref->pos.x - targetMov.x < -(float)Range)
				//{
				//	Move_LeftRight(dt, 0);
				//	targetMov.x = rightTarget;
				//}
				//break;
			}
				break;
			}
		
		//else
		//{
		//	switch (Stage)
		//	{
		//	case 1:
		//	{
		//		if (GOref->pos.x < targetMov.x)
		//		{
		//			Move_LeftRight(dt, 0);
		//		}
		//		else
		//		{
		//			GOref->pos.x = targetMov.x;
		//			targetMov.x = leftTarget;
		//			Stage = 2;
		//			GOref->scale.x *= -1;
		//		}
		//		break;
		//	}
		//	case 2:
		//	{
		//		if (GOref->pos.x > targetMov.x)
		//		{
		//			Move_LeftRight(dt, 1);
		//		}
		//		else
		//		{
		//			GOref->pos.x = targetMov.x;
		//			targetMov.x = rightTarget;
		//			Stage = 1;
		//			GOref->scale.x *= -1;
		//		}
		//		break;
		//	}

		//	}
		//}
	}
	else
		isDead = true;
}

