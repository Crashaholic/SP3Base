#include "Plane.h"
#include "../Application.h"

void Plane::Primary()
{
	for (unsigned int i = 0; i < priprojectiles.size(); ++i)
	{
		if (!priprojectiles[i])
		{
			GameObject* bomb = GOManager::GetInstance()->fetchGO();
			bomb->type = GameObject::PLAYER_PROJECTILE_BOMB;
			bomb->vel = GOref->vel;
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

void Plane::Secondary()
{
	if (secAmmo > 0)
	{
		++GOManager::GetInstance()->totalShots;
		GameObject* bomb = GOManager::GetInstance()->fetchGO();
		bomb->type = GameObject::PLAYER_PROJECTILE_NUKE;
		bomb->vel = GOref->vel;
		bomb->pos = GOref->pos;
		bomb->scale.Set(1, 2, 1);
		bomb->hasGravity = true;
		bomb->wrapMode = GameObject::SW_BOUNCE;
		for (int i = 0; i < MAX_TEXTURES; ++i)
			bomb->color[i].Set(1, 0, 0);
		--secAmmo;
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
		ReadInput(dt, 'A', 'D', 'Q', 'E');
		GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
		GOref->vel = GOref->dir * topSpeed;
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
		for (unsigned int i = 0; i < priprojectiles.size(); ++i)
		{
			if (priprojectiles[i])
			{
				if (!priprojectiles[i]->active || priprojectiles[i]->type !=GameObject::PLAYER_PROJECTILE_BOMB)
				{
					priprojectiles[i] = NULL;
					//break;
				}
			}
			else
				++totalremaining;
		}
		GOManager::GetInstance()->upgrade_1 = totalremaining;
		GOManager::GetInstance()->upgrade_2 = secAmmo;
	}
}

void Plane::ReadInput(double dt, char left, char right, char pri, char sec)
{
	if (Application::IsKeyPressed(left))
		GOref->angle += turnSpeed * (float)dt;
	else if (Application::IsKeyPressed(right))
		GOref->angle -= turnSpeed * (float)dt;

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
	//else if (Application::IsKeyPressed(sec))
	//	Secondary();
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
	GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
	topSpeed = 10.0f;
	turnSpeed = 5.0f;
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
	for (int i = 0; i < priAmmo-1; ++i)
	{
		priprojectiles.push_back(NULL);
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
		priprojectiles.push_back(NULL);
	}
	priAmmo += num;
}

void Plane::AddSec(int num)
{
	secAmmo += num;
}
