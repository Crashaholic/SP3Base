
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
			if (go->hasGravity)
				go->vel += Vector3(0.0f, -9.8f, 0.0f) * static_cast<float>(dt);

			for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;
				if (go2->active)
				{
					GameObject *goA = go;
					GameObject *goB = go2;

					if (!checkCube(goB->type))
					{
						if (!checkCube(goA->type))
						{
							continue;
						}
						goA = go2;
						goB = go;
					}
					if (checkcollision(goA, goB))
					{
						collisionresponse(goA, goB);
					}
				}
			}
			go->Update(dt);
			go->pos += go->vel * static_cast<float>(dt);
		}
	}
}

bool GOManager::checkcollision(GameObject * go1, GameObject * go2)
{
	switch (go1->type)
	{
	case GameObject::GO_BALL:
	{
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

		break;
	}
	case GameObject::GO_CUBE:
	{
		go1->perp = go1->norm.Cross(Vector3(0, 0, 1));
		go2->perp = go2->norm.Cross(Vector3(0, 0, 1));

		Vector3 hori1 = go1->norm * go1->scale.x;
		Vector3 vert1 = go1->perp * go1->scale.y;
		Vector3 hori2 = go2->norm * go2->scale.x;
		Vector3 vert2 = go2->perp * go2->scale.y;

		go1->corn[0] = go1->pos - hori1 - vert1;
		go1->corn[1] = go1->pos + hori1 - vert1;
		go1->corn[2] = go1->pos + hori1 + vert1;
		go1->corn[3] = go1->pos - hori1 + vert1;

		go2->corn[0] = go2->pos - hori2 - vert2;
		go2->corn[1] = go2->pos + hori2 - vert2;
		go2->corn[2] = go2->pos + hori2 + vert2;
		go2->corn[3] = go2->pos - hori2 + vert2;

		Vector3 normals[2];
		normals[0] = go1->norm;
		normals[1] = go1->perp;

		// Hardcoded for 2 normals - Can be converted to take in any number of normals
		for (int i = 0; i < 2; ++i)
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
	default:
	{
		break;
	}
	}
	return false;
}

void GOManager::collisionresponse(GameObject * go1, GameObject * go2)
{
	// Testing
	go1->vel = 0;
	go1->active = false;
	go2->active = false;
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
	//	if (go == newgo)
	//		return;
	//}
	//
	//for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	//{
	//	GameObject *go = (GameObject *)*it;
	//	if (go)
	//	{
	//		if (!go->active)
	//		{
	//			delete go;
	//			go = newgo;
	//			return;
	//		}
	//	}
	//	else
	//	{
	//		go = newgo;
	//		return;
	//	}
	//}
	m_goList.push_back(newgo);
}

void GOManager::cleanList()
{
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}

bool GOManager::testSAT(Vector3 axis, Vector3 corn[], float & min, float & max)
{
	// Seperating Axis Theorem Test
	min = 9001.0f;
	max = -9001.0f;

	// Hardcoded for 4 points - Can be converted to take in any number of points
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

bool GOManager::checkCube(GameObject::GAMEOBJECT_TYPE type)
{
	if (type != GameObject::PLAYER_PROJECTILE_MACHINE)
		return true;
	else
		return false;
}
