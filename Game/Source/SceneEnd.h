// Ryan
#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"

class SceneEnd : public Scene
{
public:
	SceneEnd();
	~SceneEnd();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//Mesh* meshList[NUM_GEOMETRY];

	Camera camera;

	float fps;

	// Physics
	float m_speed;
	GameObject *m_ghost;

	Button* bBack;
	std::string sBack;
	bool onButton;

	char Letter1;
	char Letter2;
	char Letter3;
	unsigned short whichChar;
	Button bArr[6];
	Button bSubmitScore;
	Button bPlayAgain;
	std::string sArrow[2];
	int choice;
	bool SubmittedScore;
};

#endif