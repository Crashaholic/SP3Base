
#include "GameObject.h"
#include "Mtx44.h"
#include "GOManager.h"

GOManager::GOManager()
{
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
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
}

void GOManager::update(double dt)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			go->Update(dt);
			go->pos += go->vel * static_cast<float>(dt);
			updateCorn(go);

			if (go->hasGravity)
			{
				go->vel += Vector3(0.0f, -9.8f, 0.0f) * static_cast<float>(dt);
			}
			//if(it != m_goList.end())
			for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;
				if (go2->active)
				{
					updateCorn(go2);
					
					if (collisionGate(go, go2))
					{
						if (checkcollision(go, go2))
						{
							collisionresponse(go, go2);
						}
					}
				}
			}
		}
	}
}

bool GOManager::checkcollision(GameObject * go1, GameObject * go2)
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

void GOManager::collisionresponse(GameObject * go1, GameObject * go2)
{

	printf("collision!\n");
}

GameObject * GOManager::fetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			go->vel.SetZero();
			go->pos.SetZero();
			go->type = GameObject::GO_NONE;
			return go;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
	return m_goList[m_goList.size() - 1];
}

std::vector<GameObject*>& GOManager::getlist()
{
	return m_goList;
}

void GOManager::addGO(GameObject * newgo)
{
	//for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	//{
	//	GameObject *go = (GameObject *)*it;
	//	if (!go->active)
	//	{
	//		delete go;
	//		go = NULL;
	//		go = newgo;
	//		return;
	//	}
	//}
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

bool GOManager::collisionGate(GameObject * go1, GameObject * go2)
{
	// collisionGate narrows down go1 and go2 to specific cases so we can avoid stupid things
	// like enemy projectiles being able to kill other enemies and player bomb destroying the player
	switch (go1->type)
	{
	// Player to Enemy + Player to Upgrade
	case GameObject::PLAYER_PLANE_KOMET:
	case GameObject::PLAYER_PLANE_A10:
	case GameObject::PLAYER_TANK:
	case GameObject::PLAYER_TANKGUN:
	{
		switch (go2->type)
		{
		case GameObject::ENEMY_PLANE_PASSIVE:
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		case GameObject::ENEMY_TANK_PASSIVE:
		case GameObject::ENEMY_TANK_AGGRESSIVE:
		case GameObject::ENEMY_BUILDING:
		case GameObject::UPGRADE_1:
		case GameObject::UPGRADE_2:
		case GameObject::UPGRADE_3:
			return true;
		}
		break;
	}
	// Player projectile to Enemy
	case GameObject::PLAYER_PROJECTILE_BOMB:
	case GameObject::PLAYER_PROJECTILE_NUKE:
	case GameObject::PLAYER_PROJECTILE_MACHINE:
	case GameObject::PLAYER_PROJECTILE_MISSILE:
	{
		switch (go2->type)
		{
		case GameObject::ENEMY_PLANE_PASSIVE:
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		case GameObject::ENEMY_TANK_PASSIVE:
		case GameObject::ENEMY_TANK_AGGRESSIVE:
		case GameObject::ENEMY_BUILDING:
			return true;
		}
		break;
	}
	// Enemy projectile to Player
	case GameObject::ENEMY_PROJECTILE_BOMB:
	case GameObject::ENEMY_PROJECTILE_MACHINE:
	{
		switch (go2->type)
		{
		case GameObject::PLAYER_PLANE_KOMET:
		case GameObject::PLAYER_PLANE_A10:
		case GameObject::PLAYER_TANK:
		case GameObject::PLAYER_TANKGUN:
			return true;
		}
		break;
	}
	}
	return false;
}