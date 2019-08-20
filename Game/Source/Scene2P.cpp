#include "Scene2P.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"
#include "Logging.h"

#include "Tank/PlayerTank.h"

Scene2P::Scene2P()
{
}

Scene2P::~Scene2P()
{
}

void Scene2P::Init()
{
	// We have to do this first before calling scene init
	cleanVar();
	GOManager::GetInstance()->wind = false;

	Scene::Init();
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
	//plane = new Plane;
	//plane = dynamic_cast<Komet*>(new Plane);
	plane = new Komet;
	plane->Init();
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();

	bLightEnabled = true;
	m_speed = 1.f;
	bulletCooldown = 0.f;
	tankSpeed = 5.f;
	Math::InitRNG();

	terr.GenerateRandomHeight(m_worldWidth);
	terr.GenerateTerrainMesh();
	GOManager::GetInstance()->terreference = &terr;
	player = new PlayerTank;
	player->Init();


	decal1 = LoadTGA("Image//A10decal2.tga");
	// Testing cubes
	/*
	Vector3 center(m_worldWidth / 2, m_worldHeight / 2, 0.0f);

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

	SpawnPos1 = vec3(-2, terr.GetHeight({-2, 0, 0}).y, 0);
	SpawnPos2 = vec3(m_worldWidth + 2, terr.GetHeight({ m_worldWidth + 2, 0, 0}).y, 0);
	spawnTimer = (float)SPAWNTIMER;

	startCount = STARTINGCOUNT;
	
	// ID for sceneEnd
	GOManager::GetInstance()->sceneID = GOManager::STYPE::FROM_2P;
	GOManager::GetInstance()->upgrade_1 = 1;
}

void Scene2P::Update(double dt)
{
	player->Update(dt);
	plane->Update(dt);
	// Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('+'))
	{
	}
	if(Application::IsKeyPressed('-'))
	{
	}
	static bool lol = false;
	if(Application::IsKeyPressed('C'))
	{
		meshList[GEO_PLAYER_PLANE_A10]->textureID[1] = decal1;
	}

	if(Application::IsKeyPressed('V'))
	{
		plane->GOref->color[0].Set(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
		plane->GOref->color[1].Set(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
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
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		
		/*
		vec3 n = terr.GetNormal(Vector3(
			static_cast<float>(x / w * m_worldWidth), 
			static_cast<float>(m_worldHeight - y / h * m_worldHeight), 
			static_cast<float>(0.0f))
		);
		LOG_NONE("Terrain Normal: % (% rads) (% deg)", n, atan2(n.y, n.x), Math::RadianToDegree(atan2(n.y, n.x)) - 90.f); //Commented out because we don't always need the information
		*/
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}
	
	// Physics Simulation Section
	fps = (float)(1.f / dt);
	m_goList = GOManager::GetInstance()->getlist();
	goWrap();
	GOManager::GetInstance()->update(dt);

	// After GOManager has updated, check for 0 lives
	if ((GOManager::GetInstance()->planeLives <= 0) ||
		(GOManager::GetInstance()->tankLives <= 0))
	{
		SceneManager::getSceneManager().switchToScene("End", this);
	}
}

void Scene2P::Render()
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
	RenderMesh(terr.tMesh, false);
	modelStack.PopMatrix();
	GLenum err = glGetError();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 0);

	// HUD
	render2PHUD();
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->planeLives), Color(0, 0, 0), 3, 4.0f, 55.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], to_string(GOManager::GetInstance()->tankLives),	Color(0, 0, 0), 3, 4.0f, 50.5f);
	//windRender();
}

void Scene2P::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	delete player;
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void Scene2P::EndWave()
{
	enemyCount = 0;
	spawnTimer = (float)SPAWNTIMER;
	waveNo++;
	LOG_WARN("LAST WAVE: %, NOW: %", waveNo - 1, waveNo);
	std::vector<GameObject*> m_goList = GOManager::GetInstance()->getlist();
	for (unsigned int i = 0; i < m_goList.size(); ++i)
	{
		GameObject* go = m_goList[i];
		if (!go->reserved)
		{
			go->active = false;
		}
	}
	//GOManager::GetInstance()->cleanList();
	terr.GenerateRandomHeight(m_worldWidth);
	terr.GenerateTerrainMesh();
	for (int i = 0; i < 5; ++i)
	{
		GameObject* building = GOManager::GetInstance()->fetchGO();
		building->type = GameObject::ENEMY_BUILDING;
		building->pos.x = Math::RandFloatMinMax(m_worldWidth / 10, m_worldWidth / 10 * 9);
		building->pos.y = terr.GetHeight(building->pos).y+3.0f;
		building->vel.SetZero();
		building->hasGravity = false;
		building->scale = Vector3(1, 1, 1)*10;
		building->norm.Set(1, 0, 0);
		//building->
	}

	//tank->pos = terr.GetHeight(tank->pos);
	//tank2->pos = terr.GetHeight(tank->pos) + vec3{0, 2, 0};
}

void Scene2P::SpawnEnemy()
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
