#ifndef GO_MANAGER_H
#define GO_MANAGER_H

#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GameObject.h"
#include <vector>

class GOManager:public Singleton<GOManager>
{
	friend Singleton<GOManager>;

private:
	std::vector<GameObject*> m_goList;

public:
	GOManager();
	~GOManager();
	void update(double dt);
	bool checkcollision(GameObject* go1, GameObject* go2);
	void collisionresponse(GameObject* go1, GameObject* go2);
	GameObject* fetchGO();
	std::vector<GameObject*>& getlist();
	void cleanList();
};

#endif