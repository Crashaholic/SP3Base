#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Material.h"
#include "Terrain.h"
struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,

		// Refer to diagram in discord
		PLAYER_PLANE_HARRIER,
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
		PLAYER_PROJECTILE_SHELL,

		ENEMY_PROJECTILE_BOMB,
		ENEMY_PROJECTILE_MACHINE,	// Machine gun

		EXPLOSION,

		SPAWNPOINT_ENEMY_TANK,
		SPAWNPOINT_ENEMY_PLANE,

		GO_TOTAL, //must be last
	};
	enum SCREENWRAP_MODE {
		SW_CLEAR = 0,
		SW_OFFSCREENCLEAR,
		SW_WRAP,
		SW_HYBRID,
		SW_BOUNCE
	};
	GAMEOBJECT_TYPE type;
	SCREENWRAP_MODE wrapMode;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	// To reset the player position
	Vector3 defaultPos;

	// To audit initial explosion radius
	Vector3 defaultScale;

	Vector3 /*pop*/corn[4];	// corner//Puns good
	Vector3 norm;			// normal
	Vector3 perp;			// normal perpendicular
	Vector3 dir;

	double Iframes;//Invincibility

	bool reserved;
	bool active;
	float exRadius;
	float angle;
	bool hasGravity;
	bool hasCollider();

	virtual void Update(double dt);
	virtual void Init();

	void reset();
	float transparency;

	Component color[MAX_TEXTURES];
	bool isColorable[MAX_TEXTURES];

	bool hasLifeTime;
	double lifeTime;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();
};

#endif