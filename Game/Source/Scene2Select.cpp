#include "Scene2Select.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"
#include "Logging.h"

Scene2Select::Scene2Select()
{
}

Scene2Select::~Scene2Select()
{
}

void Scene2Select::Init()
{
	Scene::Init();
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	Vector3 center(m_worldWidth / 2, m_worldHeight / 2 - 20.0f, 0.0f);
	for (int i = 0; i < NUM_2PBUTTON; ++i)
	{
		bPArray[i] = new Button;
		addButton(bPArray[i]);
	}
	for (int i = 0; i < NUM_2TBUTTON; ++i)
	{
		bTArray[i] = new Button;
		addButton(bTArray[i]);
	}

	planeColor[0].Set(1, 1, 1);
	planeColor[1].Set(1, 1, 1);

	tankColor[0].Set(1, 1, 1);
	tankColor[1].Set(1, 1, 1);

	bPArray[0]->init(Vector3(center.x, center.y + 60.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f)); // keep
	bPArray[1]->init(Vector3(center.x - 69.0f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f)); //plane selection
	bPArray[2]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bPArray[3]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bPArray[4]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 4.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bPArray[5]->init(Vector3(center.x - 69.0f, m_worldHeight - 3.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f)); // keep

	bTArray[0]->init(Vector3(center.x, center.y + 60.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f)); // keep
	bTArray[1]->init(Vector3(center.x + 69.0f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f)); //tank selection
	bTArray[2]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bTArray[3]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bTArray[4]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 4.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bTArray[5]->init(Vector3(center.x - 69.0f, m_worldHeight - 3.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f)); // keep

	for (int i = 0; i < NUM_2PSLIDER; ++i)
	{
		slPArray[i] = new Button;
		addButton(slPArray[i]);
	}
	for (int i = 0; i < NUM_2TSLIDER; ++i)
	{
		slTArray[i] = new Button;
		addButton(slTArray[i]);
	}

	slPArray[0]->init(Vector3(center.x - 29.f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slPArray[1]->init(Vector3(center.x - 29.f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slPArray[2]->init(Vector3(center.x - 29.f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slPArray[3]->init(Vector3(center.x - 29.0f, center.y + 14.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slPArray[4]->init(Vector3(center.x - 29.0f, center.y + 7.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slPArray[5]->init(Vector3(center.x - 29.0f, center.y + 0.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	slTArray[0]->init(Vector3(center.x + 29.f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slTArray[1]->init(Vector3(center.x + 29.f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slTArray[2]->init(Vector3(center.x + 29.f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slTArray[3]->init(Vector3(center.x + 29.0f, center.y + 14.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slTArray[4]->init(Vector3(center.x + 29.0f, center.y + 7.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slTArray[5]->init(Vector3(center.x + 29.0f, center.y + 0.5f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	planes[0] = GEO_PLAYER_PLANE_KOMET;
	decals[0][0] = 0;//Decals option 0 is... No decals
	decals[0][1] = LoadTGA("Image//Kometdecal1.tga");
	decals[0][2] = LoadTGA("Image//Kometdecal2.tga");

	planes[1] = GEO_PLAYER_PLANE_A10;
	decals[1][0] = 0;
	decals[1][1] = LoadTGA("Image//A10decal1.tga");
	decals[1][2] = LoadTGA("Image//A10decal2.tga");

	tanks[0] = GEO_PLAYER_TANK;
	decals[2][0] = 0;//Decals option 0 is... No decals
	//decals[0][1] = LoadTGA("Image//Kometdecal1.tga");
	//decals[0][2] = LoadTGA("Image//Kometdecal2.tga");

	sPArray[0] = "Start";
	sPArray[1] = "< Plane";
	sPArray[2] = "Plane >";
	sPArray[3] = "< Decal";
	sPArray[4] = "Decal >";
	sPArray[5] = "Back";

	sTArray[0] = "Start";
	sTArray[1] = "< Tank";
	sTArray[2] = "Tank >";
	sTArray[3] = "< Decal";
	sTArray[4] = "Decal >";
	sTArray[5] = "Back";

	bLightEnabled = true;
	Math::InitRNG();
	currentDecal1 = 0;
	currentPlane = 0;
}

void Scene2Select::Update(double dt)
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

	// Switch scene
	checkSwitch();

	// Mouse Section
	static bool bLButtonState = false;
	if (Application::IsMousePressed(0))
	{
		switch (choice1 - NUM_2PBUTTON)
		{
		case 0:
			planeColor[0].r = slPArray[choice1 - NUM_2PBUTTON]->checkMouseLR();
			break;
		case 1:
			planeColor[0].g = slPArray[choice1 - NUM_2PBUTTON]->checkMouseLR();
			break;
		case 2:
			planeColor[0].b = slPArray[choice1 - NUM_2PBUTTON]->checkMouseLR();
			break;
		case 3:
			planeColor[1].r = slPArray[choice1 - NUM_2PBUTTON]->checkMouseLR();
			break;
		case 4:
			planeColor[1].g = slPArray[choice1 - NUM_2PBUTTON]->checkMouseLR();
			break;
		case 5:
			planeColor[1].b = slPArray[choice1 - NUM_2PBUTTON]->checkMouseLR();
			break;
		default:
			break;
		}
		switch (choice2 - NUM_2TBUTTON)
		{
		case 0:
			tankColor[0].r = slTArray[choice2 - NUM_2TBUTTON]->checkMouseLR();
			break;
		case 1:
			tankColor[0].g = slTArray[choice2 - NUM_2TBUTTON]->checkMouseLR();
			break;
		case 2:
			tankColor[0].b = slTArray[choice2 - NUM_2TBUTTON]->checkMouseLR();
			break;
		case 3:
			tankColor[1].r = slTArray[choice2 - NUM_2TBUTTON]->checkMouseLR();
			break;
		case 4:
			tankColor[1].g = slTArray[choice2 - NUM_2TBUTTON]->checkMouseLR();
			break;
		case 5:
			tankColor[1].b = slTArray[choice2 - NUM_2TBUTTON]->checkMouseLR();
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
		std::string planename = "";
		switch (choice1)
		{
		case 0:
			SceneManager::planeChoice = planes[currentPlane];
			switch (currentPlane)
			{
			case 0:
				planename = "Komet";
				break;
			case 1:
				planename = "A10";
				break;
			}
			SceneManager::planeDecalChoice = "Image//" + planename + "decal" + std::to_string(currentDecal1) + ".tga";
			SceneManager::planeColor[0] = planeColor[0];
			SceneManager::planeColor[1] = planeColor[1];
			meshList[planes[currentPlane]]->textureID[1] = decals[currentPlane][currentDecal1];
			SceneManager::getSceneManager().switchToScene("Plane", this);
			break;
		case 1:
			--currentPlane;
			break;
		case 2:
			++currentPlane;
			break;
		case 3:
			--currentDecal1;
			break;
		case 4:
			++currentDecal1;
			break;
		case 5:
			SceneManager::getSceneManager().switchToScene("Menu", this);
			break;
		default:
			break;
		}
		std::string tankname = "";
		switch (choice2)
		{
		case 0:
			SceneManager::tankChoice = tanks[currentTank];
			switch (currentTank)
			{
			case 0:
				tankname = "";
				break;
			case 1:
				tankname = "";
				break;
			}
			SceneManager::tankDecalChoice = "Image//" + tankname + "decal" + std::to_string(currentDecal2) + ".tga";
			SceneManager::tankColor[0] = tankColor[0];
			SceneManager::tankColor[1] = tankColor[1];
			meshList[tanks[currentTank]]->textureID[1] = decals[currentTank][currentDecal2];
			SceneManager::getSceneManager().switchToScene("Plane", this);
			break;
		case 1:
			--currentTank;
			break;
		case 2:
			++currentTank;
			break;
		case 3:
			--currentDecal2;
			break;
		case 4:
			++currentDecal2;
			break;
		case 5:
			SceneManager::getSceneManager().switchToScene("Menu", this);
			break;
		default:
			break;
		}
	}
	currentPlane = Math::Wrap(currentPlane, 0, MAX_PLANES - 1);
	currentDecal1 = Math::Wrap(currentDecal1, 0, MAX_PDECALS);

	currentTank = Math::Wrap(currentTank, 0, MAX_TANKS - 1);
	currentDecal2 = Math::Wrap(currentDecal2, 0, MAX_TDECALS);

	meshList[planes[currentPlane]]->textureID[1] = decals[currentPlane][currentDecal1];

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
	for (int i = 0; i < NUM_2PBUTTON + NUM_2PSLIDER; ++i)
	{
		if (i < NUM_2PBUTTON)
		{
			if (bPArray[i]->checkMouse())
			{
				choice1 = i;
			}
			else
			{
				++inactive;
			}
		}
		else
		{
			if (slPArray[i - NUM_2PBUTTON]->checkMouse())
			{
				choice1 = i;
			}
			else
			{
				++inactive;
			}
		}
		if (inactive == NUM_2PBUTTON + NUM_2PSLIDER)
		{
			choice1 = -1;
		}
	}

	int inactive2 = 0;
	for (int i = 0; i < NUM_2TBUTTON + NUM_2TSLIDER; ++i)
	{
		if (i < NUM_2TBUTTON)
		{
			if (bTArray[i]->checkMouse())
			{
				choice2 = i;
			}
			else
			{
				++inactive2;
			}
		}
		else
		{
			if (slTArray[i - NUM_2TBUTTON]->checkMouse())
			{
				choice2 = i;
			}
			else
			{
				++inactive2;
			}
		}
		if (inactive2 == NUM_2TBUTTON + NUM_2TSLIDER)
		{
			choice2 = -1;
		}
	}

	// Physics Simulation Section
	fps = (float)(1.f / dt);
}

void Scene2Select::Render()
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

	for (int i = 0; i < NUM_2PBUTTON; ++i)
	{
		RGButtonRender(bPArray[i], sPArray[i]);
	}
	for (int i = 0; i < NUM_2PSLIDER; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(slPArray[i]->getPos().x, slPArray[i]->getPos().y, slPArray[i]->getPos().z);
		modelStack.PushMatrix();

		modelStack.Scale(slPArray[i]->getScale().x, slPArray[i]->getScale().y, slPArray[i]->getScale().z);
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
		modelStack.Translate((slPArray[i]->getPreviousOutput()*slPArray[i]->getScale().x * 2) - (slPArray[i]->getScale().x), 0, 0);
		modelStack.Scale(1.0f, slPArray[i]->getScale().y, 1.0f);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}

	for (int i = 0; i < NUM_2TBUTTON; ++i)
	{
		RGButtonRender(bTArray[i], sTArray[i]);
	}
	for (int i = 0; i < NUM_2TSLIDER; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(slTArray[i]->getPos().x, slTArray[i]->getPos().y, slTArray[i]->getPos().z);
		modelStack.PushMatrix();

		modelStack.Scale(slTArray[i]->getScale().x, slTArray[i]->getScale().y, slTArray[i]->getScale().z);
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
		modelStack.Translate((slTArray[i]->getPreviousOutput()*slTArray[i]->getScale().x * 2) - (slTArray[i]->getScale().x), 0, 0);
		modelStack.Scale(1.0f, slTArray[i]->getScale().y, 1.0f);
		RenderMesh(meshList[GEO_CUBE], false);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

	}

	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 4, m_worldHeight - 30, 0);
	modelStack.Scale(5, 5, 5);
	switch (planes[currentPlane])
	{
	case GEO_PLAYER_PLANE_KOMET:
		modelStack.Scale(4.4f, 1.8f, 1.0f);
		break;
	case GEO_PLAYER_PLANE_A10:
		modelStack.Scale(5.7f, 1.4f, 1.0f);
		break;
	}
	defaultShader.SetVec3("coloredTexture[0]", vec3{ planeColor[0].r,planeColor[0].g,planeColor[0].b });
	defaultShader.SetVec3("coloredTexture[1]", vec3{ planeColor[1].r,planeColor[1].g,planeColor[1].b });
	std::cout << planeColor[0].r << "-" << planeColor[0].g << "-" << planeColor[0].b << std::endl;
	std::cout << planeColor[1].r << "-" << planeColor[1].g << "-" << planeColor[1].b << std::endl;
	//LOG_NONE("Color: %", (Vector3)planeColor[0]);
	RenderMesh(meshList[planes[currentPlane]], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth - 40, m_worldHeight - 30, 0);
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

	renderButton();
}

void Scene2Select::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
