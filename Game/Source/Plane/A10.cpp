#include "A10.h"



void A10::Init()
{
	Plane::Init();
	turnSpeed = 5.0f;
	type = PLAYER_PLANE_A10;
	priAmmo = 30;
}

void A10::Primary()
{
	if (!fire && reload <= 0.0)
	{
		fire = true;
		currentSpray = priAmmo;
	}
}

void A10::Secondary()
{
	//if (secAmmo > 0)
	{
		//--secAmmo;
		GameObject* missile = GOManager::GetInstance()->fetchGO();
		missile->type = GameObject::PLAYER_PROJECTILE_MISSILE;
		if(dir.x>=0)
			missile->vel = Vector3(dir.y, -dir.x);
		else
			missile->vel = Vector3(-dir.y, dir.x);
		missile->vel += vel;
		missile->dir = dir;
		missile->pos = pos;
		missile->scale.Set(1, 2, 1);
		missile->hasGravity = true;
		missile->wrapMode = SW_CLEAR;
		missile->mass = 40.f;
		for (int i = 0; i < MAX_TEXTURES; ++i)
			missile->color[i] = color[i];

	}
}

void A10::Update(double dt)
{
	Plane::Update(dt);

	reload -= dt;
	if (currentSpray >0)
	{
		if(reload<=0.0)
		{
			reload = reloadSpeed;
			--currentSpray;
		GameObject* bullet = GOManager::GetInstance()->fetchGO();
		bullet->type = GameObject::PLAYER_PROJECTILE_MACHINE;
		float bulletangle = angle;
		bulletangle += Math::RandFloatMinMax(-Math::DegreeToRadian(inaccuracy), Math::DegreeToRadian(inaccuracy));
		bullet->vel = Vector3(cos(bulletangle), sin(bulletangle), 0) * 100;
		bullet->pos = pos;
		bullet->scale.Set(1, 2, 1);
		bullet->hasGravity = false;
		bullet->wrapMode = SW_CLEAR;
		bullet->mass = 20.f;
		for (int i = 0; i < MAX_TEXTURES; ++i)
			bullet->color[i] = color[i];
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
	
}

A10::A10()
{
}

A10::~A10()
{
}
