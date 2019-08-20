#pragma once
#include <Vector3.h>

struct GameObject;

struct EnemyBase
{
	bool isDead = false;
	vec3 targetMov;
	vec3 targetPos;
	float m_worldWidth;
	GameObject* playerGO;
	void SetPlayerGORef(GameObject* ref) { playerGO = ref; }

	virtual void Update(double dt) = 0;

	vec3 GetTarget() const { return this->targetPos; }
	//Where the enemy should move to
	void MoveTo(vec3 newPos) { targetMov = newPos; }
	//Where the enemy should fire at
	void FireAt(vec3 newTar) { targetPos = newTar; }
};