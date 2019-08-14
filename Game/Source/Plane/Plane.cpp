#include "Plane.h"
#include "../Application.h"

void Plane::Primary()
{
	//GameObject* bomb = GOManager::GetInstance()->fetchGO();
	
}

void Plane::Secondary()
{
}

void Plane::Update(double dt)
{
	ReadInput(dt, 'A', 'D', 'Q', 'E');
	dir = Vector3(cos(angle), sin(angle), 0);
	vel = dir * topSpeed;
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
	pos.Set(50, 50, 0);
	vel.SetZero();
	scale.Set(5.7, 1.4, 1);
	active = true;
	type = GO_A10;
	GOManager::GetInstance()->addGO(this);
}
Plane::Plane()
{

}

Plane::~Plane()
{

}