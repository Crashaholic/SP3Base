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
	void init();
	void update(double dt);
	bool checkcollision(GameObject* go1, GameObject* go2);
	void collisionresponse(GameObject* go1, GameObject* go2);
	GameObject* fetchGO();
	std::vector<GameObject*>& getlist();
	void addGO(GameObject* go);

	void cleanList();
	bool testSAT(Vector3 axis, Vector3 corn[], float &min, float &max);
	bool overlap(float min1, float max1, float min2, float max2);
	void updateCorn(GameObject* go);
	bool collisionGate(GameObject* go1, GameObject* go2);
};

#endif