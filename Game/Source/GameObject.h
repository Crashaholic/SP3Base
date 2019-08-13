#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
	//virtual void CollisionResponse(GameObject* other);
	//virtual void Update(double dt);
	//virtual void Render();
	//inline void SetPos(const Vector3& pos);
	//inline Vector3 GetPos();

	//inline void SetScale(const Vector3& _value) { scale = _value; };
	//inline Vector3 GetScale() { return scale; };


};

#endif