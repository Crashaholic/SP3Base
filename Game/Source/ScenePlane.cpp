#include "zcustompch.h"
#include "ScenePlane.h"
#include "Application.h"
#include "SceneManager.h"
#include "Tank/PlayerTank.h"

ScenePlane::ScenePlane()
{
}

ScenePlane::~ScenePlane()
{
}

void ScenePlane::Init()
{
	Scene::Init();
	waveNo = 0;
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
	//plane = new Plane;
	//plane = dynamic_cast<Komet*>(new Plane);
	switch (SceneManager::planeChoice)
	{
	default:
	case GEO_PLAYER_PLANE_KOMET:
		plane = new Komet;
		break;
	case GEO_PLAYER_PLANE_A10:
		plane = new A10;
		break;
	case GEO_PLAYER_PLANE_HARRIER:
		plane = new Harrier;
		break;
	}
	plane->Init();
	plane->GOref->color[0].Set(SceneManager::planeColor[0].r, SceneManager::planeColor[0].g, SceneManager::planeColor[0].b);
	plane->GOref->color[1].Set(SceneManager::planeColor[1].r, SceneManager::planeColor[1].g, SceneManager::planeColor[1].b);
	
	meshList[SceneManager::planeChoice]->textureID[1] = LoadTGA(SceneManager::planeDecalChoice.c_str());
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();

	bLightEnabled = true;
	m_speed = 1.f;
	bulletCooldown = 0.f;
	tankSpeed = 5.f;
	Math::InitRNG();

	terr = new Terrain();

	terr->GenerateRandomHeight(m_worldWidth);
	terr->GenerateTerrainMesh();
	GOManager::GetInstance()->terreference = terr;

	meshList[SceneManager::tankChoice]->textureID[1] = LoadTGA(SceneManager::tankDecalChoice.c_str());

	decal1 = LoadTGA("Image//A10decal2.tga");
	// Testing cubes
	Vector3 center(m_worldWidth / 2, m_worldHeight / 2, 0.0f);
	/*
	GameObject *g1 = GOManager::GetInstance()->fetchGO();
	g1->scale.Set(8.0f, 8.0f, 8.0f);
	g1->type = GameObject::GO_CUBE;
	g1->angle = 45.0f;
	g1->norm.Set(cos(Math::DegreeToRadian(g1->angle)), sin(Math::DegreeToRadian(g1->angle)), 0.0f);
	g1->vel.Set(0, 0, 0);
	g1->pos.Set(center.x, center.y, center.z);

	GameObject *g2 = GOManager::GetInstance()->fetchGO();
	g2->scale.Set(4.0f, 4.0f, 1.0f);
	g2->type = GameObject::GO_CUBE;
	g2->angle = 90.0f;
	g2->norm.Set(cos(Math::DegreeToRadian(g2->angle)), sin(Math::DegreeToRadian(g2->angle)), 0.0f);
	g2->pos.Set(center.x, center.y + 40.0f, center.z);
	*/
	SpawnPos1 = vec3(-2, terr->GetHeight({-2, 0, 0}).y, 0);
	SpawnPos2 = vec3(m_worldWidth + 2, terr->GetHeight({ m_worldWidth + 2, 0, 0}).y, 0);
	spawnTimer = (float)SPAWNTIMER;

	startCount = STARTINGCOUNT;

	// ID for sceneEnd
	GOManager::GetInstance()->sceneID = GOManager::STYPE::FROM_PLANE;
	cleanVar();
	meshList[GEO_ENEMY_TANK_AGGRESSIVE]->textureID[1] = LoadTGA(("Image//Turrettankdecal" + std::to_string(SceneManager::decalChoiceT) + ".tga").c_str());
}

void ScenePlane::Update(double dt)
{
	plane->Update(dt);
	for (uint32_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->Update(dt);
	}
	// Keyboard Section
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

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		/*
		vec3 n = terr->GetNormal(Vector3(
			static_cast<float>(x / w * m_worldWidth),
			static_cast<float>(m_worldHeight - y / h * m_worldHeight),
			static_cast<float>(0.0f))
		);
		LOG_NONE("Terrain Normal: % (% rads) (% deg)", n, atan2(n.y, n.x), Math::RadianToDegree(atan2(n.y, n.x)) - 90.f); //Commented out because we don't always need the information
		*/
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

	// Physics Simulation Section
	fps = (float)(1.f / dt);
	m_goList = GOManager::GetInstance()->getlist();
	goWrap();
	GOManager::GetInstance()->update(dt);

	// After GOManager has updated, check for 0 lives
	if (GOManager::GetInstance()->planeLives <= 0)
	{
		SceneManager::getSceneManager()->switchToScene("End", this);
	}
	spawnTimer = Math::Max(spawnTimer - dt, ((double)0.0f));

	if (spawnTimer == 0 && Math::RandFloatMinMax((float)ENEMYSPAWNCHNCRANGE_MIN, (float)ENEMYSPAWNCHNCRANGE_MAX) > (float)ENEMYSPAWNCHNC)
	{
		if (!SpawnEnemy())
			if (!GOManager::GetInstance()->checkForEnemy() && plane->GOref->pos.y > m_worldHeight / 2)
			{
				EndWave();
			}
	}
}

void ScenePlane::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2,0.0f);
	modelStack.Scale(m_worldWidth/2, m_worldHeight/2, 1.0f);
	RenderMesh(meshList[GEO_SKYBG], false);
	modelStack.PopMatrix();

	RenderMesh(meshList[GEO_AXES], false);

	std::vector<GameObject*> m_goList = GOManager::GetInstance()->getlist();
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject*)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//RenderGO(tank);
	//RenderGO(tank2);


	modelStack.PushMatrix();
	RenderMesh(terr->tMesh, false);
	modelStack.PopMatrix();
	GLenum err = glGetError();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 0);

	// HUD
	render1PHUD();
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->planeLives),		Color(0, 0, 0), 3,	4.0f,							55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(plane->getPri()),							Color(0, 0, 0), 3,	4.0f + HUD_TXT_SPACING,			55.5f);
	//RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->upgrade_1),	Color(0, 0, 0), 3,	4.0f + HUD_TXT_SPACING,			55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->upgrade_2),		Color(0, 0, 0), 3,	4.0f + HUD_TXT_SPACING * 2.0f,	55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->planeKills),		Color(0, 0, 0), 3,	4.0f + HUD_TXT_SPACING * 3.0f,	55.5f);
	windRender();
}

void ScenePlane::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	delete plane;
	glDeleteVertexArrays(1, &m_vertexArrayID);
	while (enemyList.size() > 0)
	{
		TankEnemy *tenk = enemyList.back();//heheh #3
		delete tenk;
		enemyList.pop_back();
	}
	while (buttonList.size() > 0)
	{
		Button *butt = buttonList.back();//heheh #2
		delete butt;
		buttonList.pop_back();
	}
	if (terr->tMesh != nullptr)
	{
		delete terr->tMesh;
		terr->tMesh = nullptr;
	}

	delete terr;
	terr = NULL;
}

void ScenePlane::EndWave()
{
	enemyCount = 0;
	spawnTimer = (float)SPAWNTIMER;
	waveNo++;
	LOG_WARN("LAST WAVE: %, NOW: %", waveNo - 1, waveNo);
	std::vector<GameObject*> m_goList = GOManager::GetInstance()->getlist();
	for (unsigned int i = 0; i < m_goList.size(); ++i)
	{
		GameObject* go = m_goList[i];
		if (!go->reserved && go->type != GameObject::UPGRADE_1&& go->type != GameObject::UPGRADE_2&& go->type != GameObject::UPGRADE_3)
		{
			go->active = false;
		}
	}
	//GOManager::GetInstance()->cleanList();
	terr->GenerateRandomHeight(m_worldWidth);
	terr->GenerateTerrainMesh();
	for (int i = 0; i < 5; ++i)
	{
		GameObject* building = GOManager::GetInstance()->fetchGO();
		building->type = GameObject::ENEMY_BUILDING;
		building->pos.x = Math::RandFloatMinMax(m_worldWidth / 10, m_worldWidth / 10 * 9);
		building->pos.y = terr->GetHeight(building->pos).y+3.0f;
		building->vel.SetZero();
		building->hasGravity = false;
		building->scale = Vector3(1, 1, 1)*10;
		building->norm.Set(1, 0, 0);
	}
}

bool ScenePlane::SpawnEnemy()
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
		bool spawned = false;
		for (unsigned int i = 0; i < enemyList.size(); ++i)
		{
			if (enemyList[i]->isDead)
			{
				delete enemyList[i];
				enemyList[i] = new TankEnemy({ temp.x, temp.y, temp.z }, plane->GOref, m_worldWidth, SceneManager::tankColor[0], SceneManager::tankColor[1]);
				//enemyList[i] = new TankEnemy({ temp.x, Math::RandFloatMinMax(50.f, 70.f), temp.z }, plane->GOref, m_worldWidth);
				spawned = true;
				break;
			}
		}
		if (!spawned)
			enemyList.push_back(new TankEnemy({ temp.x, temp.y, temp.z }, plane->GOref, m_worldWidth, SceneManager::tankColor[0], SceneManager::tankColor[1]));
		LOG_NONE("SPAWNED %/% AT: %", enemyCount + 1, tempcount + 1, (int)spawner + 1);
		++enemyCount;
		spawnTimer = (float)SPAWNTIMER;
	}
	return true;
}
