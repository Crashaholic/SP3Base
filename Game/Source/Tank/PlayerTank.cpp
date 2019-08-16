#include "PlayerTank.h"
#include "../Application.h"


PlayerTank::PlayerTank()
{
}


PlayerTank::~PlayerTank()
{
}

void PlayerTank::Init()
{
	tankSpeed = 5;
	SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->type = GameObject::PLAYER_TANK;
	GOref->scale.Set(3.5f, 1.4f, 1.0f);
	GOref->norm.Set(cos(Math::DegreeToRadian(GOref->angle)), sin(Math::DegreeToRadian(GOref->angle)), 0.0f);
	GOref->pos.Set(10, 30, 0);
	GOref->defaultPos = GOref->pos;
}

void PlayerTank::Update(double dt)
{

	// Tank Movement
	Terrain* terreference = GOManager::GetInstance()->terreference;
	vec3 n = terreference->GetNormal(this->GOref->pos);

	if (Application::IsKeyPressed('J')) // Left
	{
		this->GOref->pos -= tankSpeed * static_cast<float>(dt);
		this->GOref->pos.y = terreference->GetHeight(this->GOref->pos).y;

		//tank2->pos -= tankSpeed * static_cast<float>(dt);
		//tank2->pos.y = terreference.GetHeight(this->GOref->pos).y + 2;

		if (n <= 0)
		{
			this->GOref->angle = Math::RadianToDegree(terreference->GetNormal(this->GOref->pos).x);
		}
		else
			this->GOref->angle = Math::RadianToDegree(-(terreference->GetNormal(this->GOref->pos).x));
	}
	if (Application::IsKeyPressed('L')) // Right
	{
		this->GOref->pos += tankSpeed * static_cast<float>(dt);
		this->GOref->pos.y = terreference->GetHeight(this->GOref->pos).y;

		//tank2->pos += tankSpeed * static_cast<float>(dt);
		//tank2->pos.y = terreference.GetHeight(this->GOref->pos).y + 2;

		if (n <= 0)
		{
			this->GOref->angle = Math::RadianToDegree(terreference->GetNormal(this->GOref->pos).x);
		}
		else
			this->GOref->angle = Math::RadianToDegree(-(terreference->GetNormal(this->GOref->pos).x));
	}
	this->GOref->pos.x = Math::Clamp(this->GOref->pos.x, 4.f, 173.f);
	GOManager::GetInstance()->terreference;
	//tank2->pos.x = Math::Clamp(tank2->pos.x, 4.f, 173.f);
	// Tank barrel control
	//if (Application::IsKeyPressed('I')) // Left
	//{
	//	tank2->angle += 30.0f * static_cast<float>(dt);
	//}
	//if (Application::IsKeyPressed('P')) // Right
	//{
	//	tank2->angle -= 30.0f * static_cast<float>(dt);
	//}
	//tank2->angle = Math::Clamp(tank2->angle, 30.f, 150.f);
	this->GOref->norm.Set(cos(Math::DegreeToRadian(this->GOref->angle)), sin(Math::DegreeToRadian(this->GOref->angle)), 0.0f);
	//tank2->norm.Set(cos(Math::DegreeToRadian(tank2->angle)), sin(Math::DegreeToRadian(tank2->angle)), 0.0f);

	// Tank shoot
	bulletCooldown -= dt;
	//tank2->dir.Set(cosf(tank2->angle), sinf(tank2->angle), 0);
	if (Application::IsKeyPressed('N') && bulletCooldown <= 0)
	{
		GameObject *object = GOManager::GetInstance()->fetchGO();
		object->active = true;
		object->type = GameObject::PLAYER_PROJECTILE_SHELL;
		object->scale.Set(0.4f, 0.4f, 0.4f);
		//object->pos = tank2->pos;
		//object->vel = tank2->norm * BULLET_SPEED;
		object->hasGravity = false;
		if (GOManager::GetInstance()->upgrade_1 == 0)
			bulletCooldown = 1.2f;
		if (GOManager::GetInstance()->upgrade_1 == 1)
			bulletCooldown = 0.5f;
	}

}

void PlayerTank::move_leftright()
{
}

void PlayerTank::SetGORef(GameObject * GOref)
{
	this->GOref = GOref;
}
