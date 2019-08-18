#include "SceneEnd.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"

SceneEnd::SceneEnd()
{
}

SceneEnd::~SceneEnd()
{
}

void SceneEnd::Init()
{
	Scene::Init();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Junk
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	bLightEnabled = true;
	m_speed = 1.f;
	m_gravity.Set(0, -9.8f, 0);
	Math::InitRNG();
	m_ghost = new GameObject(GameObject::GO_BALL);
	m_ghost->active = true;

	Vector3 center(m_worldWidth / 2, m_worldHeight / 2 - 20.0f, 0.0f);
	bBack = new Button;
	addButton(bBack);
	bBack->init(Vector3(center.x - 69.0f, m_worldHeight - 3.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	sBack = "Back";
	onButton = false;
}

void SceneEnd::Update(double dt)
{
	//Keyboard Section
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('+'))
	{
	}
	if (Application::IsKeyPressed('-'))
	{
	}
	if (Application::IsKeyPressed('c'))
	{
	}
	if (Application::IsKeyPressed(' '))
	{
	}
	if (Application::IsKeyPressed('v'))
	{
	}

	// Switch scene
	checkSwitch();

	if (bBack->checkMouse())
		onButton = true;
	else
		onButton = false;

	// Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		if (onButton)
			SceneManager::getSceneManager().switchToScene("Menu", this);
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	// Physics Simulation Section
	fps = (float)(1.f / dt);
}

void SceneEnd::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);

	std::ostringstream s1;
	std::ostringstream s2;
	std::ostringstream s3;

	switch (GOManager::GetInstance()->sceneID)
	{
	case GOManager::STYPE::FROM_PLANE:
	{
		s1 << "Kills    : ";
		s2 << "Accuracy : ";
		s3 << "Score    : ";
		s1 << GOManager::GetInstance()->planeKills;
		s2 << GOManager::GetInstance()->planeAccuracy;
		s3 << GOManager::GetInstance()->planeHighscore;
		break;
	}
	case GOManager::STYPE::FROM_TANK:
	{
		s1 << "Kills    : ";
		s2 << "Accuracy : ";
		s3 << "Score    : ";
		s1 << GOManager::GetInstance()->tankKills;
		s2 << GOManager::GetInstance()->tankAccuracy;
		s3 << GOManager::GetInstance()->tankHighscore;
		break;
	}
	case GOManager::STYPE::FROM_2P:
	{
		s1 << "Plane Kills : ";
		s2 << "Tank	Kills  : ";
		s1 << GOManager::GetInstance()->planeKills;
		s2 << GOManager::GetInstance()->tankKills;
		break;
	}
	default:
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], s1.str(), Color(1, 1, 1), 3, 0, 12);
	RenderTextOnScreen(meshList[GEO_TEXT], s2.str(), Color(1, 1, 1), 3, 0, 9);
	RenderTextOnScreen(meshList[GEO_TEXT], s3.str(), Color(1, 1, 1), 3, 0, 6);

	RGButtonRender(bBack, sBack);
}

void SceneEnd::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);

	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
