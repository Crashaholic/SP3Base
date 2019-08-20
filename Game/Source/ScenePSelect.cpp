#include "ScenePSelect.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SceneManager.h"

#include "Logging.h"

ScenePSelect::ScenePSelect()
{
}

ScenePSelect::~ScenePSelect()
{
}

void ScenePSelect::Init()
{
	Scene::Init();
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	Vector3 center(m_worldWidth / 2, m_worldHeight / 2 - 20.0f, 0.0f);
	for (int i = 0; i < NUM_PBUTTON; ++i)
	{
		bArray[i] = new Button;
		addButton(bArray[i]);
	}

	planeColor[0].Set(1, 1, 1);
	planeColor[1].Set(1, 1, 1);
	
	bArray[0]->init(Vector3(center.x , center.y+ 60.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[1]->init(Vector3(center.x - 69.0f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[2]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[3]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[4]->init(Vector3(center.x - 69.0f, center.y - 7.0f * 4.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	bArray[5]->init(Vector3(center.x - 69.0f, m_worldHeight - 3.5f , 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	for (int i = 0; i < NUM_PSLIDER; ++i)
	{
		slArray[i] = new Button;
		addButton(slArray[i]);
	}

	slArray[3]->init(Vector3(center.x + 69.0f, center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[4]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[5]->init(Vector3(center.x + 69.0f, center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	slArray[0]->init(Vector3(center.x , center.y - 7.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[1]->init(Vector3(center.x , center.y - 7.0f * 2.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
	slArray[2]->init(Vector3(center.x , center.y - 7.0f * 3.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));

	// Purchase button
	bArray[6]->init(Vector3(center.x, center.y + 50.0f, 1.0f), Vector3(20.0f, 3.5f, 1.0f));
															  
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	
	planes[0] = GEO_PLAYER_PLANE_KOMET;
	decals[0][0] = 0;//Decals option 0 is... No decals
	decals[0][1] = LoadTGA("Image//Kometdecal1.tga");
	decals[0][2] = LoadTGA("Image//Kometdecal2.tga");

	planes[1] = GEO_PLAYER_PLANE_A10;
	decals[1][0] = 0;
	decals[1][1] = LoadTGA("Image//A10decal1.tga");
	decals[1][2] = LoadTGA("Image//A10decal2.tga");

	planes[2] = GEO_PLAYER_PLANE_HARRIER;
	decals[2][0] = 0;
	decals[2][1] = LoadTGA("Image//Harrierdecal1.tga");
	decals[2][2] = LoadTGA("Image//Harrierdecal2.tga");

	sArray[0] = "Start";
	sArray[1] = "< Plane";
	sArray[2] = "Plane >";
	sArray[3] = "< Decal";
	sArray[4] = "Decal >";
	sArray[5] = "Back";
	sArray[6] = "Purchase";

	bLightEnabled = true;
	Math::InitRNG();
	currentDecal = 0;
	currentPlane = 0;
}

void ScenePSelect::Update(double dt)
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
		switch (choice - NUM_PBUTTON)
		{
		case 0:
			planeColor[0].r = slArray[choice - NUM_PBUTTON]->checkMouseLR();
			break;
		case 1:
			planeColor[0].g = slArray[choice - NUM_PBUTTON]->checkMouseLR();
			break;
		case 2:
			planeColor[0].b = slArray[choice - NUM_PBUTTON]->checkMouseLR();
			break;
		case 3:
			planeColor[1].r = slArray[choice - NUM_PBUTTON]->checkMouseLR();
			break;
		case 4:
			planeColor[1].g = slArray[choice - NUM_PBUTTON]->checkMouseLR();
			break;
		case 5:
			planeColor[1].b = slArray[choice - NUM_PBUTTON]->checkMouseLR();
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
		switch (choice)
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
			SceneManager::planeDecalChoice = "Image//" + planename + "decal" + std::to_string(currentDecal) + ".tga";
			SceneManager::planeColor[0] = planeColor[0];
			SceneManager::planeColor[1] = planeColor[1];
			meshList[planes[currentPlane]]->textureID[1] = decals[currentPlane][currentDecal];

			if (SceneManager::planeUnlock[currentPlane] == true)
			{
				SceneManager::getSceneManager().switchToScene("Plane", this);
			}
			
			break;
		case 1:
			--currentPlane;
			break;
		case 2:
			++currentPlane;
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
		case 6:
			// Purchase plane
			if (SceneManager::money > SceneManager::planeCost[currentPlane])
			{
				SceneManager::money -= SceneManager::planeCost[currentPlane];
				SceneManager::planeUnlock[currentPlane] = true;
			}
			break;
		default:
			break;
		}
	}
	currentPlane = Math::Wrap(currentPlane, 0, MAX_PLANES-1);
	currentDecal = Math::Wrap(currentDecal, 0, MAX_PDECALS);
	meshList[planes[currentPlane]]->textureID[1] = decals[currentPlane][currentDecal];
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
	for (int i = 0; i < NUM_PBUTTON + NUM_PSLIDER; ++i)
	{
		if (i < NUM_PBUTTON)
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
			if (slArray[i - NUM_PBUTTON]->checkMouse())
			{
				choice = i;
			}
			else
			{
				++inactive;
			}
		}
		if (inactive == NUM_PBUTTON + NUM_PSLIDER)
		{
			choice = -1;
		}
	}
	// Physics Simulation Section
	fps = (float)(1.f / dt);
}

void ScenePSelect::Render()
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

	for (int i = 0; i < NUM_PBUTTON; ++i)
	{
		// Render purchase button only when the current plane is not bought
		if (i == 6)
		{
			if (SceneManager::planeUnlock[currentPlane] == true)
			{
				continue;
			}
		}
		RGButtonRender(bArray[i], sArray[i]);
	}
	for (int i = 0; i < NUM_PSLIDER; ++i)
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
			modelStack.Translate((slArray[i]->getPreviousOutput()*slArray[i]->getScale().x*2) - (slArray[i]->getScale().x), 0, 0);
			modelStack.Scale(1.0f, slArray[i]->getScale().y, 1.0f);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
		
	}
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
	modelStack.Scale(5, 5, 5);
	switch (planes[currentPlane])
	{
	case GEO_PLAYER_PLANE_KOMET:
		modelStack.Scale(4.4f, 1.8f, 1.0f);
		break;
	case GEO_PLAYER_PLANE_A10:
		modelStack.Scale(5.7f, 1.4f, 1.0f);
		break;
	case GEO_PLAYER_PLANE_HARRIER:
		modelStack.Scale(6.4f, 2.0f, 1.0f);
		break;
	}
	defaultShader.SetVec3("coloredTexture[0]", vec3{ planeColor[0].r,planeColor[0].g,planeColor[0].b });	
	defaultShader.SetVec3("coloredTexture[1]", vec3{ planeColor[1].r,planeColor[1].g,planeColor[1].b });
	std::cout << planeColor[0].r << "-" << planeColor[0].g << "-" << planeColor[0].b << std::endl;
	std::cout << planeColor[1].r << "-" << planeColor[1].g << "-" << planeColor[1].b << std::endl;
	//LOG_NONE("Color: %", (Vector3)planeColor[0]);
	RenderMesh(meshList[planes[currentPlane]], false);
	modelStack.PopMatrix();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		defaultShader.SetVec3("coloredTexture[" + std::to_string(i) + "]", vec3{ 1.f,1.f,1.f });
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "Monies:", Color(0, 0, 0), 3, 66, 54);
	std::ostringstream monies;
	monies << "$" << SceneManager::money;
	RenderTextOnScreen(meshList[GEO_TEXT], monies.str(), Color(0, 0, 0), 3, 66, 51);

	// Render cost of plane
	if (SceneManager::planeUnlock[currentPlane] == false)
	{
		std::ostringstream cost;
		cost << "$" << SceneManager::planeCost[currentPlane];
		RenderTextOnScreen(meshList[GEO_TEXT], cost.str(), Color(0.3f, 0, 0), 3, 36, 40);
	}

	//RenderTextOnScreen(meshList[GEO_TEXT], "Kinematics", Color(0, 1, 0), 3, 0, 0);
	//renderButton();
}

void ScenePSelect::Exit()
{
	// As we exit the scene, write the new data to the txt file
	SceneManager::getSceneManager().writeMonies(SceneManager::money, SceneManager::planeUnlock[1], SceneManager::planeUnlock[2]);

	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);

}
