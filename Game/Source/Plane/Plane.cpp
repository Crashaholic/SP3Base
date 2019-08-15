#include "Plane.h"
#include "../Application.h"

void Plane::Primary()
{
	//GameObject* bomb = GOManager::GetInstance()->fetchGO();
	topSpeed = 0.01f;
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
void Plane::SetTerrain(Terrain * terr)
{
	terreference = terr;
}
Plane::Plane()
{

}

Plane::~Plane()
{

}