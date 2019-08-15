#include "Plane.h"
#include "../Application.h"

void Plane::Primary()
{
	GameObject* bomb = GOManager::GetInstance()->fetchGO();
	bomb->type = GameObject::PLAYER_PROJECTILE_BOMB;
	bomb->vel = vel;
	bomb->pos = pos;
	bomb->scale.Set(1, 2, 1);
	bomb->hasGravity = true;
	bomb->wrapMode = SW_CLEAR;
	bomb->mass = 50;//RMB that this will be affected by Upgrade 1(mass is used to determine explosion radius)
	for(int i = 0;i<MAX_TEXTURES;++i)
		bomb->color[i] = color[i];
	//bomb = dynamic_cast<GameObject*>(bomb);
}

void Plane::Secondary()
{
	topSpeed = 10.0f;
}

void Plane::Update(double dt)
{
	//GameObject::Update(dt);
	ReadInput(dt, 'A', 'D', 'Q', 'E');
	dir.Set(cos(angle), sin(angle), 0.0f);
	vel = dir * topSpeed;
	pos += vel * (float)dt;
	norm = dir;
	if (dir.x < 0)
	{
		scale.y = -1.4f;
	}
	else
	{
		scale.y = 1.4f;
	}
}

void Plane::ReadInput(double dt, char left, char right, char pri, char sec)
{
	if (Application::IsKeyPressed(left))
		angle += turnSpeed * (float)dt;
	else if (Application::IsKeyPressed(right))
		angle -= turnSpeed * (float)dt;

	static bool press1, press2;
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

void Plane::Init()
{
	angle = 0.0f;
	dir.Set(cos(angle), sin(angle), 0.0f);
	topSpeed = 10.0f;
	turnSpeed = 5.0f;
	pos.Set(10.0f, 80.0f, 0.0f);
	defaultPos = pos;
	vel.SetZero();
	scale.Set(5.7f, 1.4f, 1.0f);
	active = true;
	type = PLAYER_PLANE_A10;
	wrapMode = SW_HYBRID;
	GOManager::GetInstance()->addGO(this);
}

Plane::Plane()
{

}

Plane::~Plane()
{

}