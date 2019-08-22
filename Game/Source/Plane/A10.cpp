#include "zcustompch.h"
#include "A10.h"



void A10::Init()
{
	Plane::Init();
	GOref->type = GameObject::PLAYER_PLANE_A10;
	priAmmo = 3;
	GOref->scale.Set(5.7f, 1.4f, 1.0f);
}

void A10::Primary()
{
	if (!fire && reload <= 0.0)
	{
		fire = true;
		currentSpray = priAmmo;
		GOManager::GetInstance()->totalShots += 1;
	}
}

void A10::Secondary()
{
	if (secAmmo > 0)
	{
		GOManager::GetInstance()->playSound("PShootMissile");
		GameObject* missile = GOManager::GetInstance()->fetchGO();
		missile->type = GameObject::PLAYER_PROJECTILE_MISSILE;
		if(GOref->dir.x>=0)
			missile->vel = Vector3(GOref->dir.y, -GOref->dir.x);
		else
			missile->vel = Vector3(-GOref->dir.y, GOref->dir.x);
		missile->vel += GOref->vel;
		missile->dir = GOref->dir;
		missile->pos = GOref->pos;
		missile->scale.Set(1, 2, 1);
		missile->hasGravity = true;
		missile->wrapMode = GameObject::SW_CLEAR;
		for (int i = 0; i < MAX_TEXTURES; ++i)
			missile->color[i] = GOref->color[i];
		--secAmmo;
		--GOManager::GetInstance()->upgrade_2;

		GOManager::GetInstance()->totalShots += 1;
	}
}

void A10::Update(double dt)
{	
	if (GOref->active)
	{
		Plane::Update(dt);
		priAmmo = GOManager::GetInstance()->upgrade_1 + 3;
		reload -= dt;
		if (currentSpray > 0)
		{
			if (reload <= 0.0)
			{
				GOManager::GetInstance()->playSound("PShoot");
				reload = reloadSpeed;
				--currentSpray;
				GameObject* bullet = GOManager::GetInstance()->fetchGO();
				bullet->type = GameObject::PLAYER_PROJECTILE_MACHINE;
				float bulletangle = GOref->angle;
				bulletangle += Math::RandFloatMinMax(-Math::DegreeToRadian(inaccuracy), Math::DegreeToRadian(inaccuracy));
				bullet->vel = Vector3(cos(bulletangle), sin(bulletangle), 0) * 100;
				bullet->pos = GOref->pos;
				bullet->scale.Set(0.1f, 0.1f, 1);
				bullet->hasGravity = true;
				bullet->wrapMode = GameObject::SW_CLEAR;
				for (int i = 0; i < MAX_TEXTURES; ++i)
					bullet->color[i] = GOref->color[i];
			}
		}
		else
		{
			if (fire)
			{
				fire = false;
				reload = sprayRecoverTime;
			}
		}
		GOManager::GetInstance()->upgrade_2 = secAmmo;
	}
}

A10::A10()
{
}

A10::~A10()
{
}
