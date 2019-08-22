// Ryan
#ifndef SCENE_SCORE_H
#define SCENE_SCORE_H

#include "Scene.h"
#include "HighscoreSystem.h"

class SceneScore : public Scene
{
public:
	SceneScore();
	~SceneScore();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	float fps;

	Button backButt; // heheh

	std::vector<Score> scoresPlane;
	std::vector<Score> scoresTank;

};

#endif