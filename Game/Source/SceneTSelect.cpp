#include "SceneTSelect.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"

SceneTSelect::SceneTSelect()
{
}

SceneTSelect::~SceneTSelect()
{
}

void SceneTSelect::Init()
{
	Scene::Init();
	glClearColor(0.6f, 0.6f, 0.6f, 0.0f);

	Vector3 center(m_worldWidth / 2, m_worldHeight / 2 - 20.0f, 0.0f);
	for (int i = 0; i < NUM_PBUTTON; ++i)
	{
		bArray[i] = new Button;
		addButton(bArray[i]);
	}

	tankColor[0].Set(1, 1, 1);
	tankColor[1].Set(1, 1, 1);

	bArray[0]->init(Vector3(center.x, center.y + 60.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[1]->init(Vector3(center.x - 69.0f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[2]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[3]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[4]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 4.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[5]->init(Vector3(center.x - 69.0f, m_worldHeight - 3.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	for (int i = 0; i < NUM_TSLIDER; ++i)
	{
		slArray[i] = new Button;
		addButton(slArray[i]);
	}

	slArray[3]->init(Vector3(center.x + 69.0f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[4]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[5]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	slArray[0]->init(Vector3(center.x, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[1]->init(Vector3(center.x, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[2]->init(Vector3(center.x, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	tanks[0] = GEO_PLAYER_TANK;
	decals[0][0] = 0;//Decals option 0 is... No decals
	decals[0][1] = LoadTGA("Image//Turrettankdecal1.tga");
	decals[0][2] = LoadTGA("Image//Turrettankdecal2.tga");


	sArray[0] = "Start";
	sArray[1] = "< Tank";
	sArray[2] = "Tank >";
	sArray[3] = "< Decal";
	sArray[4] = "Decal >";
	sArray[5] = "Back";

	bLightEnabled = true;
	Math::InitRNG();
}

void SceneTSelect::Update(double dt)
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

	// Mouse Section
	static bool bLButtonState = false;
	if (Application::IsMousePressed(0))
	{
		switch (choice - NUM_TBUTTON)
		{
		case 0:
			tankColor[0].r = slArray[choice - NUM_TBUTTON]->checkMouseLR();
			break;
		case 1:
			tankColor[0].g = slArray[choice - NUM_TBUTTON]->checkMouseLR();
			break;
		case 2:
			tankColor[0].b = slArray[choice - NUM_TBUTTON]->checkMouseLR();
			break;
		case 3:
			tankColor[1].r = slArray[choice - NUM_TBUTTON]->checkMouseLR();
			break;
		case 4:
			tankColor[1].g = slArray[choice - NUM_TBUTTON]->checkMouseLR();
			break;
		case 5:
			tankColor[1].b = slArray[choice - NUM_TBUTTON]->checkMouseLR();
			break;
		default:
			break;
		}

	}

	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::string tankname = "";
		switch (choice)
		{
		case 0:
			SceneManager::tankChoice = tanks[currentTank];
			switch (currentTank)
			{
			case 0:
				tankname = "Turrettank";
				break;
			case 1:
				tankname = "";
				break;
			}
			SceneManager::tankDecalChoice = "Image//" + tankname + "decal" + std::to_string(currentDecal) + ".tga";
			SceneManager::tankColor[0] = tankColor[0];
			SceneManager::tankColor[1] = tankColor[1];
			meshList[tanks[currentTank]]->textureID[1] = decals[currentTank][currentDecal];
			SceneManager::getSceneManager().switchToScene("Tank", this);
			break;
		case 1:
			--currentTank;
			break;
		case 2:
			++currentTank;
			break;
		case 3:
			--currentDecal;
			break;
		case 4:
			++currentDecal;
			break;
		case 5:
			SceneManager::getSceneManager().switchToScene("Menu", this);
			break;
		default:
			break;
		}
		switch (choice)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			GOManager::GetInstance()->playSound("Select");
		}
	}
	currentTank = Math::Wrap(currentTank, 0, MAX_TANKS - 1);
	currentDecal = Math::Wrap(currentDecal, 0, MAX_TDECALS);
	meshList[tanks[currentTank]]->textureID[1] = decals[currentTank][currentDecal];

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}

	int inactive = 0;
	for (int i = 0; i < NUM_TBUTTON + NUM_TSLIDER; ++i)
	{
		if (i < NUM_TBUTTON)
		{
			if (bArray[i]->checkMouse())
			{
				choice = i;
			}
			else
			{
				++inactive;
			}
		}
		else
		{
			if (slArray[i - NUM_TBUTTON]->checkMouse())
			{
				choice = i;
			}
			else
			{
				++inactive;
			}
		}
		if (inactive == NUM_TBUTTON + NUM_TSLIDER)
		{
			choice = -1;
		}
	}

	// Physics Simulation Section
	fps = (float)(1.f / dt);
}

void SceneTSelect::Render()
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

	for (int i = 0; i < NUM_TBUTTON; ++i)
	{
		RGButtonRender(bArray[i], sArray[i]);
	}
	for (int i = 0; i < NUM_TSLIDER; ++i)
	{
		modelStack.PushMatrix();

		modelStack.Translate(slArray[i]->getPos().x, slArray[i]->getPos().y, slArray[i]->getPos().z);


		modelStack.PushMatrix();

		modelStack.Scale(slArray[i]->getScale().x, slArray[i]->getScale().y, slArray[i]->getScale().z);
		switch (i % 3)
		{
		case 0:
			RenderMesh(meshList[GEO_GRADIENTR], false);
			break;
		case 1:
			RenderMesh(meshList[GEO_GRADIENTG], false);
			break;
		case 2:
			RenderMesh(meshList[GEO_GRADIENTB], false);
			break;
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate((slArray[i]->getPreviousOutput()*slArray[i]->getScale().x * 2) - (slArray[i]->getScale().x), 0, 0);
		modelStack.Scale(1.0f, slArray[i]->getScale().y, 1.0f);
		RenderMesh(meshList[GEO_CUBE], false);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
	modelStack.Scale(5, 5, 5);
	switch (tanks[currentTank])
	{
	case GEO_PLAYER_TANK:
		modelStack.Scale(4.0f, 2.2f, 1.0f);
		break;
	}
	defaultShader.SetVec3("coloredTexture[0]", vec3{ tankColor[0].r,tankColor[0].g,tankColor[0].b });
	defaultShader.SetVec3("coloredTexture[1]", vec3{ tankColor[1].r,tankColor[1].g,tankColor[1].b });
	RenderMesh(meshList[tanks[currentTank]], false);
	RenderMesh(meshList[GEO_PLAYER_TANKGUN], false);

	modelStack.PopMatrix();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		defaultShader.SetVec3("coloredTexture[" + std::to_string(i) + "]", vec3{ 1.f,1.f,1.f });
	}
}

void SceneTSelect::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);

}
