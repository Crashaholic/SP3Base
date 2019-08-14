#include "ScenePlane.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"
#include "Logging.h"

ScenePlane::ScenePlane()
{
}

ScenePlane::~ScenePlane()
{
}

void ScenePlane::Init()
{
	Scene::Init();
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();

	bLightEnabled = true;
	m_speed = 1.f;
	m_gravity.Set(0, -9.8f, 0); 
	bulletCooldown = 0.f;
	Math::InitRNG();

	terr.GenerateRandomHeight(static_cast<unsigned int>(m_worldWidth));
	terr.GenerateTerrainMesh();
	decal1 = LoadTGA("Image//A10decal2.tga");

	// Testing cubes
	Vector3 center(m_worldWidth / 2, m_worldHeight / 2, 0.0f);

	GameObject *g1 = GOManager::GetInstance()->fetchGO();
	g1->scale.Set(8.0f, 8.0f, 8.0f);
	g1->type = GameObject::GO_CUBE;
	g1->angle = 0.0f;
	g1->norm.Set(cos(Math::DegreeToRadian(g1->angle)), sin(Math::DegreeToRadian(g1->angle)), 0.0f);
	g1->vel.Set(0, 0, 0);
	g1->pos.Set(center.x, center.y, center.z);

	GameObject *g2 = GOManager::GetInstance()->fetchGO();
	g2->scale.Set(8.0f, 1.0f, 1.0f);
	g2->type = GameObject::GO_CUBE;
	g2->angle = 90.0f;
	g2->norm.Set(cos(Math::DegreeToRadian(g2->angle)), sin(Math::DegreeToRadian(g2->angle)), 0.0f);
	g2->vel.Set(0, -10, 0);
	g2->pos.Set(center.x, center.y + 50.0f, center.z);

	// Tank
	tank = new GameObject(GameObject::PLAYER_TANK_GENERIC);
	tank->scale.Set(15.0f, 4.0f, 1.0f);
	tank->active = true;
	tank->norm.Set(1, 1, 0);
	tank->pos.Set(center.x - 55.f, center.y - 28.0f, center.z);

	tank2 = new GameObject(GameObject::PLAYER_TANKGUN_GENERIC);
	tank2->scale.Set(7.0f, 2.0f, 1.0f);
	tank2->active = true;
	tank2->angle = 89;
	tank2->norm.Set(1, 1, 0);
	tank2->pos.Set(center.x - 53.f, center.y - 26.0f, center.z); 

	tank->pos.y = terr.getHeight(tank->pos).y;
	tank2->pos.y = terr.getHeight(tank->pos).y + 2;
}

void ScenePlane::Update(double dt)
{

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
		lol = !lol;
	}
	if (lol)
		meshList[GEO_A10]->textureID[1] = decal1;
	else
		meshList[GEO_A10]->textureID[1] = 0;

	if(Application::IsKeyPressed(' '))
	{
	}
	if(Application::IsKeyPressed('V'))
	{
		defaultShader.SetVec3("coloredTexture[0]", vec3{ Math::RandFloatMinMax(0.f,1.f),Math::RandFloatMinMax(0.f,1.f),Math::RandFloatMinMax(0.f,1.f) });
		defaultShader.SetVec3("coloredTexture[1]", vec3{ Math::RandFloatMinMax(0.f,1.f),Math::RandFloatMinMax(0.f,1.f),Math::RandFloatMinMax(0.f,1.f) });
	}

	// Tank Movement
	if (Application::IsKeyPressed('J')) // Left
	{
		tank->pos -= 5 * static_cast<float>(dt);
		tank->pos.y = terr.getHeight(tank->pos).y;

		tank2->pos -= 5 * static_cast<float>(dt);
		tank2->pos.y = terr.getHeight(tank->pos).y + 2;
	}
	if (Application::IsKeyPressed('L')) // Right
	{
		tank->pos += 5 * static_cast<float>(dt);
		tank->pos.y = terr.getHeight(tank->pos).y;

		tank2->pos += 5 * static_cast<float>(dt);
		tank2->pos.y = terr.getHeight(tank->pos).y + 2;
	}
	if (tank->pos.x <= 7.5f)
	{
		tank->pos.x = 7.5f;
	}
	if (tank->pos.x >= 125)
	{
		tank->pos.x = 125;
	}
	if (tank2->pos.x <= 9.5f)
	{
		tank2->pos.x = 9.5f;
	}
	if (tank2->pos.x >= 127)
	{
		tank2->pos.x = 127;
	}
	// Tank barrel control
	if (Application::IsKeyPressed('I')) // Left
	{
		tank2->angle += 1 * static_cast<float>(dt);
	}
	if (Application::IsKeyPressed('P')) // Right
	{
		tank2->angle -= 1 * static_cast<float>(dt);
	}
	if (tank2->angle <= 88.4f)
	{
		tank2->angle = 88.4f;
	}
	if (tank2->angle >= 90.7f)
	{
		tank2->angle = 90.7f;
	}
	// Tank shoot
	bulletCooldown -= 0.1f * dt; // lower = more delay
	tank2->dir.Set(cosf(tank2->angle), sinf(tank2->angle), 0);
	if (Application::IsKeyPressed('N') && bulletCooldown <= 0)
	{
		GameObject *object = GOManager::GetInstance()->fetchGO();
		object->active = true;
		object->type = GameObject::PLAYER_PROJECTILE_MACHINE;
		object->scale.Set(0.4f, 0.4f, 0.4f);
		object->pos = tank2->pos;
		object->vel = tank2->dir * BULLET_SPEED;
		bulletCooldown = 0.1f;
	}

	// Switch scene
	checkSwitch();

	// Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		vec3 n = terr.GetNormal(Vector3(
			static_cast<float>(x / w * m_worldWidth), 
			static_cast<float>(m_worldHeight - y / h * m_worldHeight), 
			static_cast<float>(0.0f))
		);

		LOG_NONE("Terrain Normal: % (% rads) (% deg)", n, atan2(n.y, n.x), Math::RadianToDegree(atan2(n.y, n.x)) - 90.f);
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	GOManager::GetInstance()->update(dt);

	// Physics Simulation Section
	fps = (float)(1.f / dt);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			// unspawn bullets when they leave screen
			if (go->type == GameObject::GO_BALL)
			{
				if (go->pos.x > m_worldWidth + go->scale.x || go->pos.y > m_worldHeight + go->scale.y
					|| go->pos.x < -go->scale.x || go->pos.y < -go->scale.y)
					go->active = false;
			}
		}
	}
}

void ScenePlane::Render()
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
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject*)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	RenderGO(tank);
	RenderGO(tank2);

	modelStack.PushMatrix();
	RenderMesh(terr.tMesh, false);
	modelStack.PopMatrix();
	GLenum err = glGetError();

	defaultShader.SetVec3("colorableTexture[0]", true);
	defaultShader.SetVec3("colorableTexture[1]", true);
	modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
		modelStack.Scale(57, 14, 1);
		RenderMesh(meshList[GEO_A10], false);
	modelStack.PopMatrix();
	defaultShader.SetVec3("colorableTexture[0]", false);
	defaultShader.SetVec3("colorableTexture[1]", false);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);

	ss << "Pos: " << tank->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 6);
	ss.str("");
	ss << "Vel: " << tank->vel;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 3);
	ss.str("");
	ss.precision(5);

	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);
}

void ScenePlane::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	if (tank)
	{
		delete tank;
		tank = NULL;
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
}
