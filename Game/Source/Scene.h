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
#include <vector>


class Scene
{
protected:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_A10,
		GEO_TANK,
		GEO_BULLET,
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

protected:
	Shader defaultShader;

	unsigned m_vertexArrayID;

	Light lights[1];


	MS modelStack;
	MS viewStack;
	MS projectionStack;

	bool bLightEnabled;
};

#endif