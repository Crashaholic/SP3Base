#pragma once
#include <Scene.h>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <MeshBuilder.h>

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Shader a;

	unsigned int VAO;
	unsigned int VBO;

	vec3 testQuad[3] = {
		vec3(0.5f, -0.5f,0.0f),
		vec3(-0.5f, -0.5f,0.0f),
		vec3(0.0f, 0.5f,0.0f),
	};
};

