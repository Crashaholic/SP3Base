#include "zcustompch.h"
#include "Plane.h"
#include "../Application.h"

void Plane::Primary()
{
	for (unsigned int i = 0; i < priProjectiles.size(); ++i)
	{
		if (!priProjectiles[i])
		{
			GOManager::GetInstance()->playSound("PBomb");
			GameObject* bomb = GOManager::GetInstance()->fetchGO();
			bomb->type = GameObject::PLAYER_PROJECTILE_BOMB;
			bomb->vel = GOref->vel;
			bomb->pos = GOref->pos;
			bomb->scale.Set(1, 2, 1);
			bomb->hasGravity = true;
			bomb->wrapMode = GameObject::SW_CLEAR;
			for (int i = 0; i < MAX_TEXTURES; ++i)
				bomb->color[i] = GOref->color[i];
			priProjectiles[i] = bomb;
			GOManager::GetInstance()->totalShots += 1;
			if (bomb->pos.y > GOManager::GetInstance()->terreference->GetHeight(bomb->pos).y+25)
			{
				//GOManager::GetInstance()->playSound("PBombFall");
			}
			break;
		}
	}
}

void Plane::Secondary()
{
	if (secAmmo > 0)
	{
		GOManager::GetInstance()->playSound("PBomb");
		GOManager::GetInstance()->totalShots += 1;
		GameObject* bomb = GOManager::GetInstance()->fetchGO();
		bomb->type = GameObject::PLAYER_PROJECTILE_NUKE;
		bomb->vel = GOref->vel;
		bomb->pos = GOref->pos;
		bomb->scale.Set(1, 2, 1);
		bomb->hasGravity = true;
		bomb->wrapMode = GameObject::SW_CLEAR;
		for (int i = 0; i < MAX_TEXTURES; ++i)
			bomb->color[i].Set(1, 0, 0);
		--secAmmo;
		--GOManager::GetInstance()->upgrade_2;
		if (bomb->pos.y > GOManager::GetInstance()->terreference->GetHeight(bomb->pos).y + 25)
		{
			//GOManager::GetInstance()->playSound("PBombFall");
		}
	}
	//bomb = dynamic_cast<GameObject*>(bomb);
}

void Plane::SetGORef(GameObject * GOref)
{
	this->GOref = GOref;
}

void Plane::Update(double dt)
{
	//GameObject::Update(dt);
	if (GOref->active)
	{
		ReadInput(dt, 'A', 'D', 'C', 'V');
		GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);

		// GOref->vel = GOref->dir * topSpeed;
		GOref->vel += GOref->dir * topSpeed * static_cast<float>(dt) * 10.0f;
		if (GOref->vel.Length() >= topSpeed)
		{
			GOref->vel = GOref->vel.Normalized() * topSpeed;
		}

		GOref->pos += GOref->vel * (float)dt;
		GOref->norm = GOref->dir;
		if (GOref->dir.x < 0)
		{
			GOref->scale.y = -1.4f;
		}
		else
		{
			GOref->scale.y = 1.4f;
		}
		int totalremaining = 0;
		for (unsigned int i = 0; i < priProjectiles.size(); ++i)
		{
			if (priProjectiles[i])
			{
				if (!priProjectiles[i]->active || priProjectiles[i]->type !=GameObject::PLAYER_PROJECTILE_BOMB)
				{
					priProjectiles[i] = NULL;
					//break;
				}
			}
			else
				++totalremaining;
		}
		priAmmo = GOManager::GetInstance()->upgrade_1 + 100;
		if (priProjectiles.size() < (unsigned int)priAmmo)
		{
			AddPri(priAmmo - (int)priProjectiles.size());
		}
		secAmmo = GOManager::GetInstance()->upgrade_2;
		//GOManager::GetInstance()->upgrade_1 = totalremaining;
		GOManager::GetInstance()->upgrade_2 = secAmmo;
	}
}

void Plane::Turn_CWCCW(double dt, bool CCW)
{
	if(CCW)
		GOref->angle += turnSpeed * (float)dt;
	else
		GOref->angle -= turnSpeed * (float)dt;

}

void Plane::ReadInput(double dt, char left, char right, char pri, char sec)
{
	if (Application::IsKeyPressed(left))
		Turn_CWCCW(dt, true);
	else if (Application::IsKeyPressed(right))
		Turn_CWCCW(dt, false);

	static bool press1 = false, press2 = false;
	if (Application::IsKeyPressed(pri) && !press1)
	{
		Primary();
		press1 = true;
	}
	else if (!Application::IsKeyPressed(pri) && press1)
	{
		press1 = false;
	}
	if (Application::IsKeyPressed(sec) && !press2)
	{
		Secondary();
		press2 = true;
	}
	else if (!Application::IsKeyPressed(sec) && press2)
	{
		press2 = false;
	}
}

int Plane::getPri()
{
	return priAmmo;
}

int Plane::getSec()
{
	return secAmmo;
}

void Plane::Init()
{	
	SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->reserved = true;
	GOref->angle = 0.0f;
	GOref->dir.Set(1.0f, 0.0f, 0.0f);
	GOref->norm.Set(1.0f, 0.0f, 0.0f);
	topSpeed = 12.0f;
	turnSpeed = 4.0f;
	GOref->pos.Set(10.0f, 80.0f, 0.0f);
	GOref->defaultPos = GOref->pos;
	GOref->vel.SetZero();
	//scale.Set(5.7f, 1.4f, 1.0f);
	GOref->scale.Set(4.4f, 1.8f, 1.0f);
	GOref->active = true;
	GOref->type = GameObject::PLAYER_PLANE_KOMET;
	GOref->wrapMode = GameObject::SW_HYBRID;
	//GOManager::GetInstance()->addGO(this);
	priAmmo = 0;
	AddPri(1);
	secAmmo = 0;
	AddSec(3);
	priProjectiles = { NULL };
	for (int i = 0; i < priAmmo-1; ++i)
	{
		priProjectiles.push_back(NULL);
	}
}

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::AddPri(int num)
{
	for (int i = 0; i < num; ++i)
	{
		priProjectiles.push_back(NULL);
	}
	//priAmmo += num;
}

void Plane::AddSec(int num)
{
	secAmmo += num;
}
