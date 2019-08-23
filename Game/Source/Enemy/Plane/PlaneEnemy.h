#pragma once

#include "../../Plane/Plane.h"
#include "../EnemyBase.h"

struct GameObject;

class PlaneEnemy : public Plane, public EnemyBase
{
public:
	bool directionleft = false;
	const static int HEIGHT_RANGE = 20;

	PlaneEnemy();
	PlaneEnemy(vec3 pos, GameObject* ref, float m_worldWidth, Color color1, Color color2);
	~PlaneEnemy();

	float cooldown;
	float cooldownLimit;

	float originalHeight;

	// ref is Player's GO reference
	void SpawnNewPlaneEnemy(vec3 pos, GameObject* ref, float m_worldWidth, Color color1, Color color2);
	void Update(double dt) override;

	virtual void Primary() override;

};

