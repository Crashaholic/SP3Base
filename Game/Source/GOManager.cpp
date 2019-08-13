
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

void GOManager::update(double dt)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->hasGravity)
				go->vel += Vector3(0, -9.8, 0) * static_cast<float>(dt);
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

//Collision Detection Codes For Ryan
bool GOManager::checkcollision(GameObject * go1, GameObject * go2)
{
	return false;
}

//Collision Response Codes For Ryan
void GOManager::collisionresponse(GameObject * go1, GameObject * go2)
{
	switch (go1->type)
	{

	}
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
			go->scale.Set(1, 1, 1);
			go->type = GameObject::GO_NONE;
			return go;
		}
	}
	unsigned int size = m_goList.size();
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
	return m_goList[size];
}

std::vector<GameObject*> GOManager::getlist()
{
	return m_goList;
}
