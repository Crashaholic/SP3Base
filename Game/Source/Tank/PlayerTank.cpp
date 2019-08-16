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
	// Tank Movement
	Terrain* terreference = GOManager::GetInstance()->terreference;
	Vector3 frontCheck = GOref->pos + Vector3(GOref->scale.x/2, 0,0);
	Vector3 rearCheck = GOref->pos - Vector3(GOref->scale.x/2, 0,0);
	if (Application::IsKeyPressed('J')) // Left
	{
		this->GOref->pos -= tankSpeed * static_cast<float>(dt);
	}
	else if (Application::IsKeyPressed('L')) // Right
	{
		this->GOref->pos += tankSpeed * static_cast<float>(dt);
	}
	GOref->angle = atan2(terreference->GetHeight(frontCheck).y - terreference->GetHeight(rearCheck).y, GOref->scale.x);
	//vec3 n = Vector3(frontCheck.x, terreference->GetHeight(frontCheck).y) - Vector3(rearCheck.x, terreference->GetHeight(rearCheck).y).Normalized();
	//vec3 n = terreference->GetNormal(this->GOref->pos);
	GOref->norm.Set(cos(GOref->angle), sin(GOref->angle), 0);
	//GOref->norm.Set(n.y, -n.x);
	//GOref->norm.Set(Math::DegreeToRadian(cos(GOref->angle)), Math::DegreeToRadian(sin(GOref->angle)));
	GOref->pos.y = (terreference->GetHeight(frontCheck).y+terreference->GetHeight(rearCheck).y)/2+ heightOffset;
	//GOref->pos.y = terreference->GetHeight(GOref->pos).y+ heightOffset;
	GOref->pos.x = Math::Clamp(GOref->pos.x, 4.f, 173.f);
	//Tank barrel control
	if (Application::IsKeyPressed('I')) // Left
	{
		turretAngle += 3.0f * static_cast<float>(dt);
	}
	if (Application::IsKeyPressed('P')) // Right
	{
		turretAngle -= 3.0f * static_cast<float>(dt);
	}
	turretAngle = Math::Clamp(turretAngle, Math::DegreeToRadian(30.f), Math::DegreeToRadian(150.f));
	GOref->dir.Set(cos(turretAngle), sin(turretAngle), 0);
	// Tank shoot
	if (bulletCooldown > 0)
	{
		bulletCooldown -= dt;
	}
	if (Application::IsKeyPressed('N') && bulletCooldown <= 0)
	{
		GameObject *object = GOManager::GetInstance()->fetchGO();
		object->active = true;
		object->type = GameObject::PLAYER_PROJECTILE_SHELL;
		object->scale.Set(0.4f, 0.4f, 0.4f);
		object->pos = GOref->pos;
		object->vel = GOref->dir * 30.0f;
		object->hasGravity = false;
		bulletCooldown = Math::Max(1.5f - (GOManager::GetInstance()->upgrade_1 * 0.2f), 0.2f);
	}
}

void PlayerTank::move_leftright()
{
}

void PlayerTank::SetGORef(GameObject * GOref)
{
	this->GOref = GOref;
}
