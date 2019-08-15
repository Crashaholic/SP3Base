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

		// Refer to diagram in discord
		PLAYER_PLANE_KOMET,
		PLAYER_PLANE_A10,
		PLAYER_TANK,
		PLAYER_TANKGUN,
		
		ENEMY_PLANE_PASSIVE,
		ENEMY_PLANE_AGGRESSIVE,
		ENEMY_TANK_PASSIVE,
		ENEMY_TANK_AGGRESSIVE,
		ENEMY_BUILDING,

		UPGRADE_1,	// Blast radius OR Reload speed
		UPGRADE_2,	// Bomb count OR Movement speed
		UPGRADE_3,	// Extra life

		PLAYER_PROJECTILE_BOMB,
		PLAYER_PROJECTILE_NUKE,
		PLAYER_PROJECTILE_MACHINE,	// Machine gun
		PLAYER_PROJECTILE_MISSILE,

		ENEMY_PROJECTILE_BOMB,
		ENEMY_PROJECTILE_MACHINE,	// Machine gun

		EXPLOSION,

		GO_TOTAL, //must be last
	};
	enum SCREENWRAP_MODE {
		SW_CLEAR = 0,
		SW_WRAP,
		SW_HYBRID,
		SW_BOUNCE
	};
	GAMEOBJECT_TYPE type;
	SCREENWRAP_MODE wrapMode;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	Vector3 /*pop*/corn[4];	// corner
	Vector3 norm;			// normal
	Vector3 perp;			// normal perpendicular

	Vector3 dir;
	bool active;
	float mass;
	float angle;
	bool hasGravity;
	virtual void Update(double dt);
	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();
};

#endif