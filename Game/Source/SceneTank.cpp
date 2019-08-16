#include "SceneTank.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"
#include <Logging.h>

SceneTank::SceneTank()
{
}

SceneTank::~SceneTank()
{
}

void SceneTank::Init()
{
	Scene::Init();
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);


	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();

	bLightEnabled = true;
	m_speed = 1.f;
	m_gravity.Set(0, -9.8f, 0);
	Math::InitRNG();
	m_ghost = new GameObject(GameObject::GO_BALL);
	m_ghost->active = true;

	terr.GenerateRandomHeight(static_cast<unsigned int>(m_worldWidth));
	terr.GenerateTerrainMesh();
	// Set terrain reference in GOManager
	GOManager::GetInstance()->terreference = &terr;

	SpawnPos1 = vec3(-2, terr.GetHeight({ -2, 0, 0 }).y, 0);
	SpawnPos2 = vec3(m_worldWidth + 2, terr.GetHeight({ m_worldWidth + 2, 0, 0 }).y, 0);
	spawnTimer = (float)SPAWNTIMER;

	startCount = STARTINGCOUNT;
}

void SceneTank::Update(double dt)
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

	spawnTimer = Math::Max(spawnTimer - dt, ((double)0.0f));

	if (spawnTimer == 0 && Math::RandFloatMinMax((float)ENEMYSPAWNCHNCRANGE_MIN, (float)ENEMYSPAWNCHNCRANGE_MAX) > (float)ENEMYSPAWNCHNC)
	{
		SpawnEnemy();
	}

	static bool hPressed = false;
	if (Application::IsKeyPressed('H'))
	{
		if (!hPressed)
		{
			EndWave();
			hPressed = true;
		}
	}
	else
	{
		if (hPressed)
			hPressed = false;
	}

	// Switch scene
	checkSwitch();

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

void SceneTank::Render()
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
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Kinematics", Color(0, 1, 0), 3, 0, 0);
}

void SceneTank::Exit()
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

void ScenePlane::EndWave()
{
	enemyCount = 0;
	spawnTimer = (float)SPAWNTIMER;
	waveNo++;
	LOG_WARN("LAST WAVE: %, NOW: %", waveNo - 1, waveNo);
	terr.GenerateRandomHeight(m_worldWidth);
	terr.GenerateTerrainMesh();
	//tank->pos = terr.GetHeight(tank->pos);
	//tank2->pos = terr.GetHeight(tank->pos) + vec3{ 0, 2, 0 };
}

void ScenePlane::SpawnEnemy()
{
	unsigned int tempcount = startCount + 1 * waveNo;
	if (enemyCount > tempcount)
	{
		return;
	}
	else
	{
		bool spawner = rand() % 2;
		//GameObject* t = GOManager::GetInstance()->fetchGO();
		//t->pos = (spawner? SpawnPos1 : SpawnPos2);
		//TODO: TANK TARGET/MOVE CODE HERE
		//HACK: DISABLED UNTIL WE HAVE MADE THE MOVE FUNCTIONS FOR SOME TANK CLASS
		LOG_NONE("SPAWNED %/% AT: %", enemyCount + 1, tempcount + 1, (int)spawner + 1);
		++enemyCount;
		spawnTimer = (float)SPAWNTIMER;
	}
}
