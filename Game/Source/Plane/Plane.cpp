#include "Plane.h"
#include "../Application.h"

void Plane::Primary()
{
	GameObject* bomb = GOManager::GetInstance()->fetchGO();
	bomb->type = GameObject::PLAYER_PROJECTILE_BOMB;
	bomb->vel = vel;
	bomb->pos = pos;
	//bomb->
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
	for (int i = 0; i < 4; ++i)
	{
	if (corn[i].y < terreference->getHeight(corn[i]).y)
		Reset();
	}
}

void Plane::ReadInput(double dt, char left, char right, char pri, char sec)
{
	if (Application::IsKeyPressed(left))
		angle += turnSpeed * (float)dt;
	else if (Application::IsKeyPressed(right))
		angle -= turnSpeed * (float)dt;

	if (Application::IsKeyPressed(pri))
		Primary();
	else if (Application::IsKeyPressed(sec))
		Secondary();
}

void Plane::Init()
{
	angle = 0.0f;
	dir.Set(cos(angle), sin(angle), 0.0f);
	topSpeed = 10.0f;
	turnSpeed = 5.0f;
	pos.Set(10.0f, 80.0f, 0.0f);
	vel.SetZero();
	scale.Set(5.7f, 1.4f, 1.0f);
	active = true;
	type = PLAYER_PLANE_A10;
	wrapMode = SW_HYBRID;
	GOManager::GetInstance()->addGO(this);
}

void Plane::Reset()
{
	angle = 0.0f;
	dir.Set(cos(angle), sin(angle), 0.0f);
	pos.Set(10.0f, 80.0f, 0.0f);
	vel.SetZero();
}

Plane::Plane()
{

}

Plane::~Plane()
{

}