#pragma once
#include <Scene.h>

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

