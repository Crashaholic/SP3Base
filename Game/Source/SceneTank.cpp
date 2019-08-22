#include "zcustompch.h"
#include "SceneTank.h"
#include "Application.h"
#include "SceneManager.h"
#include "Tank/PlayerTank.h"

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
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();

	terr = new Terrain;
	terr->GenerateRandomHeight(m_worldWidth);
	terr->GenerateTerrainMesh();
	GOManager::GetInstance()->terreference = terr;
	player = new PlayerTank;
	player->Init();
	player->GOref->color[0].Set(SceneManager::tankColor[0].r, SceneManager::tankColor[0].g, SceneManager::tankColor[0].b);
	player->GOref->color[1].Set(SceneManager::tankColor[1].r, SceneManager::tankColor[1].g, SceneManager::tankColor[1].b);

	meshList[SceneManager::tankChoice]->textureID[1] = LoadTGA(SceneManager::tankDecalChoice.c_str());

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));


	bLightEnabled = true;
	m_speed = 1.f;
	Math::InitRNG();


	SpawnPos1 = vec3(-300.f, 0, 0);
	SpawnPos2 = vec3(m_worldWidth + 300.f, 0, 0);
	spawnTimer = (float)SPAWNTIMER;

	startCount = STARTINGCOUNT;

	// ID for sceneEnd
	GOManager::GetInstance()->sceneID = GOManager::STYPE::FROM_TANK;

	cleanVar();
}

void SceneTank::Update(double dt)
{
	player->Update(dt);
	for (uint32_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->Update(dt);
	}

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
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}

	fps = (float)(1.f / dt);
	m_goList = GOManager::GetInstance()->getlist();
	goWrap();
	// Physics Simulation Section
	GOManager::GetInstance()->update(dt); 

	// After GOManager has updated, check for 0 lives
	if (GOManager::GetInstance()->tankLives <= 0)
	{
		SceneManager::getSceneManager().switchToScene("End", this);
	}
	spawnTimer = Math::Max(spawnTimer - dt, ((double)0.0f));

	if (spawnTimer == 0 && Math::RandFloatMinMax((float)ENEMYSPAWNCHNCRANGE_MIN, (float)ENEMYSPAWNCHNCRANGE_MAX) > (float)ENEMYSPAWNCHNC)
	{
		if (!SpawnEnemy())
			if (!GOManager::GetInstance()->checkForEnemy())
			{
				EndWave();
			}
	}
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

	std::vector<GameObject*> m_goList = GOManager::GetInstance()->getlist();
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject*)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	//RenderGO(tank);
	//RenderGO(tank2);

	modelStack.PushMatrix();
	RenderMesh(terr->tMesh, false);
	modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 0);

	// HUD
	render1PHUD();
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->tankLives), Color(0, 0, 0), 3, 4.0f, 55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->upgrade_1), Color(0, 0, 0), 3, 4.0f + HUD_TXT_SPACING, 55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->upgrade_2), Color(0, 0, 0), 3, 4.0f + HUD_TXT_SPACING * 2.0f, 55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->tankKills), Color(0, 0, 0), 3, 4.0f + HUD_TXT_SPACING * 3.0f, 55.5f);
	windRender();
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
	delete player;
	while (enemyList.size() > 0)
	{
		PlaneEnemy *tenk = enemyList.back();//heheh #3
		delete tenk;
		enemyList.pop_back();
	}
	if (terr->tMesh != nullptr)
	{
		delete terr->tMesh;
		terr->tMesh = nullptr;
	}
	delete terr;
	terr = NULL;
}

void SceneTank::EndWave()
{
	enemyCount = 0;
	spawnTimer = (float)SPAWNTIMER;
	waveNo++;
	LOG_WARN("LAST WAVE: %, NOW: %", waveNo - 1, waveNo);
	terr->GenerateRandomHeight(m_worldWidth);
	terr->GenerateTerrainMesh();
}

bool SceneTank::SpawnEnemy()
{
	unsigned int tempcount = startCount + 1 * waveNo;
	if (enemyCount > tempcount)
	{
		return false;
	}
	else
	{
		bool spawner = rand() % 2;
		vec3 temp = (spawner ? SpawnPos1 : SpawnPos2);
		enemyList.push_back(new PlaneEnemy({ temp.x, Math::RandFloatMinMax(m_worldHeight/2, m_worldHeight), temp.z }, player->GOref, m_worldWidth));
		LOG_NONE("SPAWNED %/% AT: %", enemyCount + 1, tempcount + 1, (int)spawner + 1);
		++enemyCount;
		spawnTimer = (float)SPAWNTIMER;
	}
	return true;
}
