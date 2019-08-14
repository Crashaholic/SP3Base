#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_A10,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	Vector3 corn[4];	// corner
	Vector3 norm;		// normal
	Vector3 perp;		// normal perpendicular

	bool active;
	float mass;
	bool hasGravity;
	virtual void Update(double dt);
	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();
};

#endif