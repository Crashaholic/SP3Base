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
	SetGORef(GOManager::GetInstance()->fetchGO());
	GOref->reserved = true;
	GOref->type = GameObject::PLAYER_TANK;
	GOref->scale.Set(4.0f, 2.2f, 1.0f);
	heightOffset = GOref->scale.y + 0.5f;
	GOref->scale *= 1.5f;
	GOref->norm.Set(cos(Math::DegreeToRadian(GOref->angle)), sin(Math::DegreeToRadian(GOref->angle)), 0.0f);
	GOref->pos.Set(10, 30, 0);
	GOref->wrapMode = GameObject::SW_WRAP;
	vec3 n = GOManager::GetInstance()->terreference->GetNormal(this->GOref->pos);
	if (n <= 0)
	{
		this->GOref->angle = Math::RadianToDegree(GOManager::GetInstance()->terreference->GetNormal(this->GOref->pos).x);
	}
	else
		this->GOref->angle = Math::RadianToDegree(-(GOManager::GetInstance()->terreference->GetNormal(this->GOref->pos).x));

	GOref->pos.y = GOManager::GetInstance()->terreference->GetHeight(GOref->pos).y;
	GOref->defaultPos = GOref->pos;
}

void PlayerTank::Update(double dt)
{
	if (GOref->active)
	{
		// Tank Movement
		tankSpeed = Math::Max(15.f + (GOManager::GetInstance()->upgrade_2 * 1.f), 15.f);
		Terrain* terreference = GOManager::GetInstance()->terreference;
		Vector3 frontCheck = GOref->pos + Vector3(GOref->scale.x / 2, 0, 0);
		Vector3 rearCheck = GOref->pos - Vector3(GOref->scale.x / 2, 0, 0);
		GOref->angle = atan2(terreference->GetHeight(frontCheck).y - terreference->GetHeight(rearCheck).y, GOref->scale.x);
		//vec3 n = Vector3(frontCheck.x, terreference->GetHeight(frontCheck).y) - Vector3(rearCheck.x, terreference->GetHeight(rearCheck).y).Normalized();
		//vec3 n = terreference->GetNormal(this->GOref->pos);
		GOref->norm.Set(cos(GOref->angle), sin(GOref->angle), 0);
		GOref->dir.Set(-GOref->norm.y, GOref->norm.x);
		GOref->pos.y = (terreference->GetHeight(frontCheck).y + terreference->GetHeight(rearCheck).y) / 2 + heightOffset;
		GOref->pos.x = Math::Clamp(GOref->pos.x, 4.f, 173.f);
		//Tank barrel control
		turretAngle = Math::Clamp(turretAngle, Math::DegreeToRadian(30.f), Math::DegreeToRadian(150.f));
		GOref->dir.Set(cos(turretAngle), sin(turretAngle), 0);
		// Tank shoot
		if (bulletCooldown > 0)
		{
			bulletCooldown -= dt;
		}
		ReadInput(dt, 'K', 'I', 'J', 'L', 'N');
	}
}


void PlayerTank::ReadInput(double dt, char left, char right, char aimleft, char aimright, char fire)
{
	if (Application::IsKeyPressed(left)) // Left
	{
		Move_LeftRight(dt, true);
	}
	else if (Application::IsKeyPressed(right)) // Right
	{
		Move_LeftRight(dt, false);
	}
	if (Application::IsKeyPressed(aimleft)) // Left
	{
		Turret_LeftRight(dt, true);
	}
	if (Application::IsKeyPressed(aimright)) // Right
	{
		Turret_LeftRight(dt, false);
	}
	if (Application::IsKeyPressed(fire))
		Fire();
}
