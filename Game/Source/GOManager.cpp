
#include "GameObject.h"
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
				if (checkcollision(go, go2))
				{
					collisionresponse(go, go2);
				}
			}
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
		// Ball to cube collision
		break;
	}
	case GameObject::GO_CUBE:
	{
		// As of now, I see no reason to move to update function since it is a single case.
		go1->corn[0] = go1->pos - (go1->perp * go1->scale.x) - (go1->norm * go1->scale.y);
		go1->corn[1] = go1->pos + (go1->perp * go1->scale.x) - (go1->norm * go1->scale.y);
		go1->corn[2] = go1->pos + (go1->perp * go1->scale.x) + (go1->norm * go1->scale.y);
		go1->corn[3] = go1->pos - (go1->perp * go1->scale.x) + (go1->norm * go1->scale.y);
		go1->axis[0] = go1->perp;
		go1->axis[1] = go1->norm;
		go1->orig[0] = go1->corn[0].Dot(go1->perp);
		go1->orig[1] = go1->corn[0].Dot(go1->norm);

		go2->corn[0] = go2->pos - (go2->perp * go2->scale.x) - (go2->norm * go2->scale.y);
		go2->corn[1] = go2->pos + (go2->perp * go2->scale.x) - (go2->norm * go2->scale.y);
		go2->corn[2] = go2->pos + (go2->perp * go2->scale.x) + (go2->norm * go2->scale.y);
		go2->corn[3] = go2->pos - (go2->perp * go2->scale.x) + (go2->norm * go2->scale.y);
		go2->axis[0] = go2->perp;
		go2->axis[1] = go2->norm;
		go2->orig[0] = go2->corn[0].Dot(go2->perp);
		go2->orig[1] = go2->corn[0].Dot(go2->norm);

		if ((overlapOBB(go1, go2) == true) && (overlapOBB(go2, go1) == true))
			return true;

		break;
	}
	default:
	{
		;
	}
	}
	return false;
}

void GOManager::collisionresponse(GameObject * go1, GameObject * go2)
{
	// Testing
	// go1->active = false;
	// go2->active = false;
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

void GOManager::cleanList()
{
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}

bool GOManager::overlapOBB(GameObject * go1, GameObject * go2)
{
	for (int i = 0; i < 2; ++i)
	{
		double t = go2->corn[i].Dot(go2->axis[i]);
		double min = t;
		double max = t;

		for (int j = 0; j < 4; ++j)
		{
			t = go2->corn[j].Dot(go2->axis[j]);

			if (t < min)
				min = t;
			else if (t > max)
				max = t;
		}

		if ((min > 1 + go1->orig[i]) || (max < go1->orig[i]))
			return false;
	}
	return true;
}
