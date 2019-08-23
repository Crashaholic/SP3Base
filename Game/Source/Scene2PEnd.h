// Ryan
#ifndef SCENE_2PEND_H
#define SCENE_2PEND_H

#include "Scene.h"

class Scene2PEnd : public Scene
{
public:
	Scene2PEnd();
	~Scene2PEnd();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//Mesh* meshList[NUM_GEOMETRY];
	float fps;

	Camera camera;

	Button* bBack;
	std::string sBack;
	bool onButton;

	Button uil;

	int choice;
};

#endif