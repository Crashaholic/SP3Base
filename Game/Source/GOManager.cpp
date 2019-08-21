
#include "GameObject.h"
#include "Mtx44.h"
#include "GOManager.h"
#include "SceneManager.h"
#include "Logging.h"

GOManager::GOManager()
{
	sceneID = NONE;
	planeLives = 2;
	tankLives = 2;
	upgrade_1 = 0;
	upgrade_2 = 0;
	attackCount = 0;
	planeKills = 0;
	tankKills = 0;
	planeAccuracy = 0.0f;
	tankAccuracy = 0.0f;
	planeHighscore = 0;
	tankHighscore = 0;
	totalHits = 0;
	totalShots = 0;
	windVector = Vector3(1, 0, 0);
	windVectorN = Vector3(1, 0, 0);
	windBT = WIND_TIMER;
	rain = false;
	wind = true;
	muted = false;
}

GOManager::~GOManager()
{
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}

void GOManager::init()
{
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("HitEnemy", "Audio//Hit enemy.mp3");
	CSoundEngine::GetInstance()->AddSound("HitTerr", "Audio//Hit terrain.wav");
	CSoundEngine::GetInstance()->AddSound("Upgrade", "Audio//Upgrade.wav");
	CSoundEngine::GetInstance()->AddSound("Select", "Audio//Selection.wav");
	CSoundEngine::GetInstance()->AddSound("TShoot", "Audio//Tank shoot.wav");

	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
}

void GOManager::update(double dt)
{
	// Wind logic
	if (windBT > 0.0)
	{
		windBT -= dt;
	}
	if (windBT <= 0.0)
	{
		windBT = WIND_TIMER;

		float windAngle = Math::RadianToDegree(atan2(
			GOManager::GetInstance()->windVector.y,
			GOManager::GetInstance()->windVector.x));

		float windAngleN = Math::RandFloatMinMax(windAngle - WIND_RANGE, windAngle + WIND_RANGE);

		windVectorN = Vector3(cos(Math::DegreeToRadian(windAngleN)), sin(Math::DegreeToRadian(windAngleN)), 0.0f);
	}
	if (wind)
	{
		windVector = (windVector + windVectorN * static_cast<float>(dt)).Normalized();
	}
	else
	{
		windVector.SetZero();
	}

	for (unsigned int i = 0; i < m_goList.size(); ++i)
	{
		GameObject *go = m_goList[i];
		if (go->active)
		{
			go->Update(dt);
			if (go->hasGravity)
			{
				go->vel += gravity * static_cast<float>(dt);
				go->vel += windVector * WIND_POWER * static_cast<float>(dt);
			}

			// Rain logic
			if (go->type == GameObject::GO_RAIN)
			{
				go->norm = go->vel;
				if ((go->pos.x < -8.0f)		||
					(go->pos.x > 184.0f)	||
					(go->pos.y < -8.0f))
				{
					go->pos.Set(
						Math::RandFloatMinMax(-50.0f - windVector.x * 100.0f, 226.0f - windVector.x * 100.0f),
						Math::RandFloatMinMax(100.0f + go->scale.y, 270.0f),
						0.0f);
					go->vel = windVector * WIND_POWER + gravity;
				}
				continue;
			}
			updateCorn(go);

			if (terrainGate(go))
			{
				if (checkTerrain(go))
				{
					terrainResponse(go);
				}
			}
			if (go->Iframes <= 0.0)
			{
				for (unsigned int j = 0; j < m_goList.size(); ++j)
				{
					GameObject *go2 = m_goList[j];
					if (go2->active)
					{
						updateCorn(go2);
						if (go2->Iframes <= 0.0)
						{
							if (collisionGate(go, go2))
							{
								if (checkCollision(go, go2))
								{
									collisionResponse(go, go2);
								}
							}
						}
					}
				}
			}
		}
	}
	if (totalShots == 0)
	{
		planeAccuracy = tankAccuracy = 0.0f;
	}
	else
	{
		planeAccuracy = (float)planeKills / (float)totalShots;
		tankAccuracy = (float)tankKills / (float)totalShots;
	}
}

bool GOManager::collisionGate(GameObject * go1, GameObject * go2)
{
	switch (go1->type)
	{
	// Player To
	case GameObject::PLAYER_PLANE_HARRIER:
	case GameObject::PLAYER_PLANE_KOMET:
	case GameObject::PLAYER_PLANE_A10:
	case GameObject::PLAYER_TANK:
	{
		// Enemy & Upgrade
		switch (go2->type)
		{
		case GameObject::ENEMY_PLANE_PASSIVE:
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		case GameObject::ENEMY_TANK_PASSIVE:
		case GameObject::ENEMY_TANK_AGGRESSIVE:
		case GameObject::UPGRADE_1:
		case GameObject::UPGRADE_2:
		case GameObject::UPGRADE_3:
		case GameObject::GO_CUBE:
		{
			return true;
		}
		// Exception: Only plane collides with building
		case GameObject::ENEMY_BUILDING:
		{
			switch (go1->type)
			{
			case GameObject::PLAYER_PLANE_HARRIER:
			case GameObject::PLAYER_PLANE_KOMET:
			case GameObject::PLAYER_PLANE_A10:
				return true;
			}
			return false;
		}
		}
		break;
	}
	// Player Plane Projectile To
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::PLAYER_PROJECTILE_NUKE:
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	{
		switch (go2->type)
		{
		// Enemy & other player
		case GameObject::ENEMY_PLANE_PASSIVE:
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		case GameObject::ENEMY_TANK_PASSIVE:
		case GameObject::ENEMY_TANK_AGGRESSIVE:
		case GameObject::ENEMY_BUILDING:
		case GameObject::PLAYER_TANK:
			return true;
		}
		break;
	}
	// Player Tank Projectile To
	case GameObject::PLAYER_PROJECTILE_SHELL:
	{
		switch (go2->type)
		{
		// Enemy & other player
		case GameObject::ENEMY_PLANE_PASSIVE:
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		// case GameObject::ENEMY_BUILDING:
		case GameObject::PLAYER_PLANE_HARRIER:
		case GameObject::PLAYER_PLANE_KOMET:
		case GameObject::PLAYER_PLANE_A10:
			return true;
		}
		break;
	}
	// Enemy Projectile to Player
	case GameObject::ENEMY_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
	{
		switch (go2->type)
		{
		case GameObject::PLAYER_PLANE_HARRIER:
		case GameObject::PLAYER_PLANE_KOMET:
		case GameObject::PLAYER_PLANE_A10:
		case GameObject::PLAYER_TANK:
			return true;
		}
		break;
	}
	}
	return false;
}

bool GOManager::checkCollision(GameObject * go1, GameObject * go2)
{
	switch (go1->type)
	{
	case GameObject::GO_BALL:
	{
		// Method I - Matrix rotation + AABB vs Circle (Bugged)
		/*
		float angle = atan2(go2->norm.y, go2->norm.x);
		Vector3 newPos = go1->pos - go2->pos;
		Mtx44 rotation;
		rotation.SetToRotation(Math::RadianToDegree(-angle) + 90.0f, 0, 0, 1);
		newPos = rotation * newPos + go2->pos;
		Vector3 len(newPos - go2->pos);
		
		if (len.x > go2->scale.x + go1->scale.x)
			return false;
		if (len.y > go2->scale.y + go1->scale.y)
			return false;
		if (len.x <= go2->scale.x + go1->scale.x)
			return true;
		if (len.y <= go2->scale.y + go1->scale.y)
			return true;
		if ((len - go2->scale).LengthSquared() <= go1->scale.x * go1->scale.x)
			return true;
		else
			return false;
		*/

		// Method II - Modified SAT test (Bugged)
		/*
		go2->perp = go2->norm.Cross(Vector3(0, 0, 1));
		Vector3 hori2 = go2->norm * go2->scale.x;
		Vector3 vert2 = go2->perp * go2->scale.y;
		go2->corn[0] = go2->pos - hori2 - vert2;
		go2->corn[1] = go2->pos + hori2 - vert2;
		go2->corn[2] = go2->pos + hori2 + vert2;
		go2->corn[3] = go2->pos - hori2 + vert2;

		float min, max;
		Vector3 dist(9001.0f, 9001.0f, 9001.0f);
		for (int i = 0; i < 4; ++i)
		{
			if ((go1->pos - go2->corn[i]).LengthSquared() < dist.LengthSquared())
				dist = go1->pos - go2->corn[i];
		}
		testSAT(dist.Normalized(), go2->corn, min, max);
		if ((dist.Length() - max - go1->scale.x > 0.0f) &&
			(dist.Length() > 0.0f))
			return false;

		Vector3 normals[2];
		normals[0] = go2->norm;
		normals[1] = go2->perp;

		for (int i = 0; i < 2; ++i)
		{
			float min1, max1, min2, max2;
			testSAT(normals[i], go1, min1, max1);
			testSAT(normals[i], go1, min2, max2);
			if (overlap(min1, max1, min2, max2) == false)
				return false;
		}
		return true;
		*/

		return false;
		break;
	}
	default:
	{
		Vector3 normals[4];
		normals[0] = go1->norm;
		normals[1] = go1->perp;
		normals[2] = go2->norm;
		normals[3] = go2->perp;

		for (int i = 0; i < 4; ++i)
		{
			float min1, max1, min2, max2;
			testSAT(normals[i], go1->corn, min1, max1);
			testSAT(normals[i], go2->corn, min2, max2);
			if (overlap(min1, max1, min2, max2) == false)
				return false;
		}
		return true;
		break;
	}
	}
	return false;
}

void GOManager::collisionResponse(GameObject * go1, GameObject * go2)
{
	switch (go1->type)
	{
	// Player
	case GameObject::PLAYER_PLANE_HARRIER:
	case GameObject::PLAYER_PLANE_KOMET:
	case GameObject::PLAYER_PLANE_A10:
	case GameObject::PLAYER_TANK:
	{
		switch (go2->type)
		{
		// if for some reason the player decides to crash into an enemy object
		case GameObject::ENEMY_PLANE_PASSIVE:
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		case GameObject::ENEMY_TANK_PASSIVE:
		case GameObject::ENEMY_TANK_AGGRESSIVE:
		case GameObject::ENEMY_BUILDING:
		{
			toExplosion(go2, false);
			LOG_TRACE("Player collided with enemy");

			// check go1 again
			switch (go1->type)
			{
			case GameObject::PLAYER_PLANE_HARRIER:
			case GameObject::PLAYER_PLANE_KOMET:
			case GameObject::PLAYER_PLANE_A10:
			{
				++planeKills;
				planeDeath(go1);
				break;
			}
			case GameObject::PLAYER_TANK:
			{
				++tankKills;
				tankDeath(go1);
				break;
			}
			}
			break;
		}
		// Upgrade pickups
		case GameObject::UPGRADE_1:
		{
			LOG_TRACE("Player picked up UPGRADE_1");
			++upgrade_1;
			playSound("Upgrade");
			go2->active = false;
			break;
		}
		case GameObject::UPGRADE_2:
		{
			LOG_TRACE("Player picked up UPGRADE_2");
			++upgrade_2;
			playSound("Upgrade");
			go2->active = false;
			break;
		}
		case GameObject::UPGRADE_3:
		{
			LOG_TRACE("Player picked up UPGRADE_3");
			++planeLives;
			++tankLives;
			playSound("Upgrade");
			go2->active = false;
			break;
		}
		// Testing
		case GameObject::GO_CUBE:
		{
			go2->active = false;
			break;
		}
		}
		break;
	}
	// Projectiles
	case GameObject::PLAYER_PROJECTILE_SHELL:
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_BOMB:
	{
		go1->exRadius = 7.0f;
		break;
	}
	case GameObject::PLAYER_PROJECTILE_NUKE:
	{
		go1->exRadius = 20.0f;
		break;
	}
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
	{
		go1->exRadius = 5.0f;
		break;
	}
	default:
	{
		break;
	}
	}
	// Check projectiles again
	switch (go1->type)
	{
	case GameObject::PLAYER_PROJECTILE_SHELL:
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_BOMB:
	case GameObject::PLAYER_PROJECTILE_NUKE:
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
		LOG_NONE("Projectile collided with object");
		enemyDeath(go2);
		playSound("HitEnemy");
		toExplosion(go1, false);
	}
}

bool GOManager::terrainGate(GameObject * go)
{
	switch (go->type)
	{
	case GameObject::PLAYER_PLANE_HARRIER:
	case GameObject::PLAYER_PLANE_KOMET:
	case GameObject::PLAYER_PLANE_A10:
	case GameObject::UPGRADE_1:
	case GameObject::UPGRADE_2:
	case GameObject::UPGRADE_3:
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::PLAYER_PROJECTILE_NUKE:
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	case GameObject::ENEMY_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
		return true;
	}
	return false;
}

bool GOManager::checkTerrain(GameObject * go)
{
	for (int i = 0; i < 4; ++i)
	{
		if (go->corn[i].y < terreference->GetHeight(go->corn[i]).y)
			return true;
	}
	return false;
}

void GOManager::terrainResponse(GameObject * go)
{
	// Handles response between game objects and terrain
	switch (go->type)
	{
	case GameObject::PLAYER_PLANE_HARRIER:
	case GameObject::PLAYER_PLANE_KOMET:
	case GameObject::PLAYER_PLANE_A10:
	{
		LOG_TRACE("Player collided with terrain");
		planeDeath(go);
		break;
	}
	case GameObject::UPGRADE_1:
	case GameObject::UPGRADE_2:
	case GameObject::UPGRADE_3:
	{
		LOG_TRACE("UPGRADE collided with terrain");
		if (go->vel.y < 0.0f)
		{
			go->vel.y = 0.0f;
		}
		break;
	}
	// Projectiles
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_BOMB:
	{
		go->exRadius = 7.0f;
		terreference->DeformTerrain(go->pos, go->exRadius);
		break;
	}
	case GameObject::PLAYER_PROJECTILE_NUKE:
	{
		go->exRadius = 20.0f;
		terreference->DeformTerrain(go->pos, go->exRadius);
		break;
	}
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
	{
		go->exRadius = 5.0f;
		terreference->DeformTerrain(go->pos, go->exRadius);
		break;
	}
	default:
	{
		break;
	}
	}
	// Check projectiles again
	switch (go->type)
	{
	case GameObject::PLAYER_PROJECTILE_SHELL:
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_BOMB:
	case GameObject::PLAYER_PROJECTILE_NUKE:
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
		LOG_NONE("Projectile collided with terrain");
		playSound("HitTerr");
		toExplosion(go, false);
	}
}

void GOManager::planeDeath(GameObject * go)
{
	LOG_ERROR("Player plane exploded");
	--planeLives;
	++tankKills;
	GameObject* ex = fetchGO();
	ex->exRadius = 10.0f;
	ex->pos = go->pos;
	terreference->DeformTerrain(ex->pos, ex->exRadius);
	go->active = false;
	toExplosion(ex, true);
	if (planeLives <= 0)
	{
		int accuracyBonus = static_cast<int>(planeAccuracy * planeKills);
		planeHighscore = planeKills + accuracyBonus;
	}
	else
	{
		go->defaultPos.x = Math::RandFloatMinMax(10.0f, 166.0f);
		go->reset();
		go->Iframes = 5.0;
		go->active = true;
	}
}

void GOManager::tankDeath(GameObject* go)
{
	LOG_ERROR("Player tank exploded");
	--tankLives;
	++planeKills;
	GameObject* ex = fetchGO();
	ex->exRadius = 10.0f;
	ex->pos = go->pos;
	terreference->DeformTerrain(ex->pos, ex->exRadius);
	go->active = false;
	toExplosion(ex, true);
	if (tankLives <= 0)
	{
		int accuracyBonus = static_cast<int>(tankAccuracy * tankKills);
		tankHighscore = tankKills + accuracyBonus;
	}
	else
	{
		go->defaultPos.x = Math::RandFloatMinMax(10.0f, 166.0f);
		go->reset();
		go->Iframes = 5.0;
		go->active = true;
	}
}

void GOManager::toExplosion(GameObject * go, bool fromPlayer)
{
	go->type = GameObject::EXPLOSION;
	go->scale = Vector3(1, 1, 1) * go->exRadius;
	go->defaultScale = go->scale;
	for (unsigned int i = 0; i < MAX_TEXTURES; ++i)
	{
		go->color[i].Set(1.f, 1.f, 1.f);
	}
	go->pos.y = Math::Max(terreference->GetHeight(go->pos).y + 0.01f, go->pos.y);
	go->vel.SetZero();
	go->hasGravity = false;
	go->hasLifeTime = true;
	go->lifeTime = 0.7;
	go->angle = Math::RandFloatMinMax(0.0f, 360.0f);
	go->norm.Set(cos(go->angle), sin(go->angle), 0.0f);
	for (unsigned int i = 0; i < m_goList.size(); ++i)
	{
		GameObject *a = m_goList[i];
		if (a == go && !a->active)
			continue;
		float length = 9001.0f;

		for (int i = 0; i < 4; i++)
		{
			if ((a->corn[i] - go->pos).Length() < length)
				length = (a->corn[i] - go->pos).Length();
		}
		if (a->Iframes <= 0.0)
		{
			if (length < go->exRadius)
			{
				exResponse(a, fromPlayer);
			}
		}
	}
}

void GOManager::exResponse(GameObject * go, bool fromPlayer)
{
	if (go->active == true)
	{
		if (fromPlayer == false)
		{
			switch (go->type)
			{
			case GameObject::PLAYER_PLANE_HARRIER:
			case GameObject::PLAYER_PLANE_KOMET:
			case GameObject::PLAYER_PLANE_A10:
			{
				planeDeath(go);
				break;
			}
			case GameObject::PLAYER_TANK:
			{
				tankDeath(go);
				break;
			}
			default:
			{
				break;
			}
			}
		}
		enemyDeath(go);
	}
}

GameObject * GOManager::fetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if ((!go->active) && (!go->reserved))
		{
			go->active = true;
			go->vel.SetZero();
			go->pos.SetZero();
			go->hasLifeTime = false;
			go->hasGravity = false;
			go->lifeTime = 0.0;
			go->transparency = 1.0f;
			for (int i = 0; i < 8; ++i)
				go->color[i].Set(1, 1, 1);
			go->wrapMode = GameObject::SW_CLEAR;
			go->type = GameObject::GO_NONE;
			return go;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
	return fetchGO();
}

GameObject * GOManager::fetchReservedGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if ((!go->active) && (go->reserved))
		{
			go->active = true;

			go->vel.SetZero();
			go->pos.SetZero();
			go->hasLifeTime = false;
			go->hasGravity = false;
			go->lifeTime = 0.0;
			go->transparency = 1.0f;
			for (int i = 0; i < 8; ++i)
				go->color[i].Set(1, 1, 1);
			go->wrapMode = GameObject::SW_CLEAR;
			go->type = GameObject::GO_NONE;
			return go;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		GameObject* go = new GameObject(GameObject::GO_NONE);
		go->reserved = true;
		m_goList.push_back(go);
	}
	return fetchReservedGO();
}

std::vector<GameObject*>& GOManager::getlist()
{
	return m_goList;
}

void GOManager::addGO(GameObject * newgo)
{
	m_goList.push_back(newgo);
}

void GOManager::cleanList()
{
	for (auto go : m_goList)
	{
		delete go;
	}
	m_goList.clear();
	m_goList.shrink_to_fit();
}

bool GOManager::testSAT(Vector3 axis, Vector3 corn[], float & min, float & max)
{
	// Seperating Axis Theorem Test
	min = 9001.0f;
	max = -9001.0f;

	for (int i = 0; i < 4; ++i)
	{
		// dot product finds length along the axis when points are projected onto it
		float x = corn[i].Dot(axis);

		// find min and max values along the axis
		if (x < min)
			min = x;
		if (x > max)
			max = x;
	}
	return false;
}

bool GOManager::overlap(float min1, float max1, float min2, float max2)
{
	if (((min2 >= min1) && (min2 <= max1)) ||
		((min1 >= min2) && (min1 <= max2)))
		return true;

	return false;
}

void GOManager::updateCorn(GameObject * go)
{
	if (go->hasCollider())
	{
		go->perp = go->norm.Cross(Vector3(0, 0, 1));
		Vector3 hori1 = go->norm * go->scale.x;
		Vector3 vert1 = go->perp * go->scale.y;
		go->corn[0] = go->pos - hori1 - vert1;
		go->corn[1] = go->pos + hori1 - vert1;
		go->corn[2] = go->pos + hori1 + vert1;
		go->corn[3] = go->pos - hori1 + vert1;
	}
}

void GOManager::enemyDeath(GameObject * go)
{
	switch (go->type)
	{
	case GameObject::ENEMY_PLANE_PASSIVE:
	case GameObject::ENEMY_PLANE_AGGRESSIVE:
	case GameObject::ENEMY_TANK_PASSIVE:
	case GameObject::ENEMY_TANK_AGGRESSIVE:
	case GameObject::ENEMY_BUILDING:
	{
		++planeKills;
		++tankKills;
		float i = Math::RandFloatMinMax(0.0f, 100.0f);
		if (i <= UPGRADE_CHANCE)
		{
			int j = Math::RandIntMinMax(0, 2);
			GameObject *up = GOManager::GetInstance()->fetchGO();
			switch (j)
			{
			case 0:
				up->type = GameObject::UPGRADE_1;
				break;
			case 1:
				up->type = GameObject::UPGRADE_2;
				break;
			case 2:
				up->type = GameObject::UPGRADE_3;
				break;
			}
			up->active = true;
			up->scale.Set(3.f, 3.f, 1.f);

			switch (sceneID)
			{
			case STYPE::FROM_PLANE:
				up->vel.y = 3.0f;
				break;
			case STYPE::FROM_TANK:
				up->vel.y = -5.0f;
				break;
			default:
				LOG_ERROR("Tried to spawn upgrade outside of appropriate scene!");
				break;
			}

			up->pos = go->pos;
			up->hasLifeTime = true;
			up->lifeTime = 10.0;
			up->norm.Set(1, 0, 0);
		}
		go->active = false;
		break;
	}
	default:
	{
		break;
	}
	}
}

void GOManager::playSound(std::string name)
{
	if (muted == false)
	{
		CSoundEngine::GetInstance()->PlayASound(name);
	}
}