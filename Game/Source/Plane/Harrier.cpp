#include "Harrier.h"
#include "../Application.h"

Harrier::Harrier()
{
	VTOLmode = false;
}

Harrier::~Harrier()
{
}

void Harrier::Primary()
{
	if (!fire && reload <= 0.0)
	{
		fire = true;
		currentSpray = priAmmo;
		GOManager::GetInstance()->totalShots += 1;
	}
}

void Harrier::Secondary()
{
	if (secAmmo > 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			GameObject* missile = GOManager::GetInstance()->fetchGO();
			missile->type = GameObject::PLAYER_PROJECTILE_MISSILE;
			if (GOref->dir.x >= 0)
				missile->vel = Vector3(GOref->dir.y, -GOref->dir.x);
			else
				missile->vel = Vector3(-GOref->dir.y, GOref->dir.x);
			missile->vel += GOref->vel;
			missile->dir = GOref->dir;
			missile->pos = GOref->pos;
			missile->scale.Set(1, 2, 1);
			missile->hasGravity = true;
			missile->wrapMode = GameObject::SW_CLEAR;
			for (int i = 0; i < MAX_TEXTURES; ++i)
				missile->color[i] = GOref->color[i];
		}
			--secAmmo;
			--GOManager::GetInstance()->upgrade_2;

			GOManager::GetInstance()->totalShots += 1;
	}
}

void Harrier::Init()
{
	Plane::Init();
	GOref->angle = 0.0f;
	turnSpeed = 3.0f;
	topSpeed = 30.0f;
	GOref->type = GameObject::PLAYER_PLANE_HARRIER;
	priAmmo = 3;
	GOref->scale.Set(6.4f, 2.0f, 1.0f);
	up.Set(0, 1, 0);
}

void Harrier::ReadInput(double dt, char left, char right, char pri, char sec, char up, char down, char vtol, char flip)
{
	if (Application::IsKeyPressed(left))
		Turn_CWCCW(dt, true);
	else if (Application::IsKeyPressed(right))
		Turn_CWCCW(dt, false);

	static bool press1 = false, press2 = false, press3 = false, press4 = false;
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
	if (Application::IsKeyPressed(vtol) && !press3)
	{
		VTOLmode = !VTOLmode;
		press3 = true;
	}
	else if (!Application::IsKeyPressed(vtol) && press3)
	{
		press3 = false;
	}

	if (VTOLmode)
	{
		if (Application::IsKeyPressed(up))
			Move_UpDown(dt, true);
		else if (Application::IsKeyPressed(down))
			Move_UpDown(dt, false);
		if (Application::IsKeyPressed(flip) && !press4)
		{
			GOref->dir = -GOref->dir;
			GOref->angle += Math::DegreeToRadian(180);
			press4 = true;
		}
		else if (!Application::IsKeyPressed(flip) && press4)
		{
			press4 = false;
		}

	}
}

void Harrier::Update(double dt)
{
	if (GOref->active)
	{
		//GOref->hasGravity = VTOLmode;
		ReadInput(dt, 'A', 'D', 'Q', 'E', 'W', 'S', 'R', 'T');
		GOref->dir.Set(cos(GOref->angle), sin(GOref->angle), 0.0f);
		if (GOref->dir.x < 0)
		{
			GOref->scale.y = -1.4f;
			up.Set(-(GOref->dir.y), (GOref->dir.x), 0);
			up *= -1;
		}
		else
		{
			GOref->scale.y = 1.4f;
			up.Set(-(GOref->dir.y), (GOref->dir.x), 0);
		}
		GOref->norm = GOref->dir;
		if(!VTOLmode)
		{
			// GOref->vel = GOref->dir * topSpeed;
			GOref->vel += GOref->dir * topSpeed * static_cast<float>(dt) * 2.0f;
			if (GOref->vel.Length() >= topSpeed)
			{
				GOref->vel = GOref->vel.Normalized() * topSpeed;
			}
		}
		else
		{
			GOref->vel += GOManager::GetInstance()->gravity * (float)dt;
			GOref->vel *= (1 - (float)(0.1 * dt));
			Vector3 hovervel = this->up * (float)dt*VTOLTopSpeed;
			//hovervel.y = Math::Min(hovervel.y, 9.8f*(float)dt);
			GOref->vel.y += hovervel.y;
		}
		//up = GOref->norm;
		//GOref->pos += GOref->vel * (float)dt;


		priAmmo = GOManager::GetInstance()->upgrade_1 + 10;
		reload -= dt;
		if (currentSpray > 0)
		{
			if (reload <= 0.0)
			{
				GOManager::GetInstance()->playSound("PShoot");
				reload = reloadSpeed;
				--currentSpray;
				GameObject* bullet = GOManager::GetInstance()->fetchGO();
				bullet->type = GameObject::PLAYER_PROJECTILE_MACHINE;
				float bulletangle = GOref->angle;
				bulletangle += Math::RandFloatMinMax(-Math::DegreeToRadian(inaccuracy), Math::DegreeToRadian(inaccuracy));
				bullet->vel = Vector3(cos(bulletangle), sin(bulletangle), 0) * 100;
				bullet->pos = GOref->pos;
				bullet->scale.Set(1, 2, 1);
				bullet->hasGravity = true;
				bullet->wrapMode = GameObject::SW_CLEAR;
				for (int i = 0; i < MAX_TEXTURES; ++i)
					bullet->color[i] = GOref->color[i];
			}
		}
		else
		{
			if (fire)
			{
				fire = false;
				reload = sprayRecoverTime;
			}
		}
		//GOManager::GetInstance()->upgrade_2 = secAmmo;
		secAmmo = GOManager::GetInstance()->upgrade_2;


	}
}

void Harrier::Move_UpDown(double dt, bool up)
{
	if (VTOLmode)
	{
		if (up)
			GOref->vel += this->up * (float)dt*VTOLTopSpeed;
		else
			GOref->vel += -this->up * (float)dt*VTOLTopSpeed;
	}
}

