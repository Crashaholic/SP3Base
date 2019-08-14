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
	dir = Vector3(cos(angle), sin(angle), 0);
	vel = dir * topSpeed;
	pos += vel * (float)dt;
	//norm.Set(-dir.y, dir.x);
	norm.Set(cos(angle), sin(angle), 0.0f);
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
	dir.Set(1, 0, 0);
	topSpeed = 10.0f;
	turnSpeed = 5.0f;
	pos.Set(50, 5, 0);
	vel.SetZero();
	scale.Set(5.7, 1.4, 1);
	scale *= 2;
	active = true;
	type = PLAYER_PLANE_A10;
	GOManager::GetInstance()->addGO(this);
}
Plane::Plane()
{

}

Plane::~Plane()
{

}