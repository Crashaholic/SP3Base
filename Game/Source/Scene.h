#ifndef SCENE_H
#define SCENE_H

#include "GOManager.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include "shader.hpp"
#include "Button/Button.h"
#include <vector>
#include <string>
#include "Tank/Tank.h"

class Scene
{
protected:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_GRADIENTR,
		GEO_GRADIENTG,
		GEO_GRADIENTB,

		GEO_PLAYER_PLANE_KOMET,
		GEO_PLAYER_PLANE_A10,
		GEO_PLAYER_TANK,
		GEO_PLAYER_TANKGUN,

		GEO_ENEMY_PLANE_PASSIVE,
		GEO_ENEMY_PLANE_AGGRESSIVE,
		GEO_ENEMY_TANK_PASSIVE,
		GEO_ENEMY_TANK_AGGRESSIVE,
		GEO_ENEMY_BUILDING,

		GEO_UPGRADE_1,					// Blast radius OR Reload speed
		GEO_UPGRADE_2,					// Bomb count OR Movement speed
		GEO_UPGRADE_3,					// Extra life

		GEO_UPGRADE_1_HUD,
		GEO_UPGRADE_2_HUD,
		GEO_UPGRADE_3_HUD,

		GEO_KILL,
		GEO_ARROW,

		GEO_PLAYER_PROJECTILE_BOMB,
		GEO_PLAYER_PROJECTILE_NUKE,
		GEO_PLAYER_PROJECTILE_MACHINE,	// Machine gun
		GEO_PLAYER_PROJECTILE_MISSILE,
		GEO_PLAYER_PROJECTILE_SHELL,

		GEO_ENEMY_PROJECTILE_BOMB,
		GEO_ENEMY_PROJECTILE_MACHINE,	// Machine gun

		GEO_EXPLOSION,
		GEO_DEBUG,						// Debug balls

		NUM_GEOMETRY,
	};	
	Mesh* meshList[NUM_GEOMETRY];

public:
	Scene() {}
	~Scene() {}

	virtual void Init();
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	bool readyExit = false;

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGO(GameObject *go);

	void checkSwitch();
	void debugBalls(GameObject *go);

	void addButton(Button* button);
	void cleanButton();
	void renderButton();

	// Rendering HUD for 1 player and 2 player modes
	void render1PHUD();
	void render2PHUD();

protected:
	Shader defaultShader;

	unsigned m_vertexArrayID;

	Light lights[1];
	std::vector<Button*>buttonList;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	bool bLightEnabled;

	float m_worldWidth;
	float m_worldHeight;

	float windAngle;

	void cleanVar();
	void RGButtonRender(Button* b, std::string s);

	const float HUD_SPACING = 15.5f;
	const float HUD_TXT_SPACING = 7.0f;
};

#endif