#include "zcustompch.h"
#include "SceneMenu.h"
#include "Application.h"

#include "SceneManager.h"

SceneMenu::SceneMenu()
{
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Init()
{
	Scene::Init();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	bLightEnabled = true;
	Math::InitRNG();

	Vector3 center(m_worldWidth / 2, m_worldHeight / 2 - 20.0f, 0.0f);

	// Buttons init
	bArray[0] = new Button;
	bArray[1] = new Button;
	bArray[2] = new Button;
	bArray[3] = new Button;
	bArray[4] = new Button;

	addButton(bArray[0]);
	addButton(bArray[1]);
	addButton(bArray[2]);
	addButton(bArray[3]);
	addButton(bArray[4]);

	bArray[0]->init(Vector3(center.x - 69.0f, center.y - 5.4f, 1.0f),						Vector3(20.0f, 3.5f, 1.0f));	
	bArray[1]->init(Vector3(center.x - 69.0f, bArray[0]->getPos().y - 7.0f, 1.0f),			Vector3(20.0f, 3.5f, 1.0f));
	bArray[2]->init(Vector3(center.x - 69.0f, bArray[0]->getPos().y - 7.0f * 2.0f, 1.0f),	Vector3(20.0f, 3.5f, 1.0f));
	bArray[3]->init(Vector3(center.x - 69.0f, bArray[0]->getPos().y - 7.0f * 4.0f, 1.0f),	Vector3(20.0f, 3.5f, 1.0f)); // not in use
	bArray[4]->init(Vector3(center.x - 69.0f, bArray[0]->getPos().y - 7.0f * 3.0f, 1.0f),	Vector3(20.0f, 3.5f, 1.0f));

	// Text init
	sArray[0] = "Play as Plane";
	sArray[1] = "Play as Tank";
	sArray[2] = "2 Player";
	sArray[3] = "Highscores"; // not in use
	sArray[4] = "Mute";

	//sound = true;
	choice = 0;

	HighScoreSystem::GetInstance()->ParseFile(0, &scoresPlane);
	HighScoreSystem::GetInstance()->ParseFile(1, &scoresTank);
}

void SceneMenu::Update(double dt)
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

	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		switch (choice)
		{
		case 0:
			SceneManager::getSceneManager()->switchToScene("PSelect", this);
			break;
		case 1:
			SceneManager::getSceneManager()->switchToScene("TSelect", this);
			break;
		case 2:
			SceneManager::getSceneManager()->switchToScene("2Select", this);
			break;
		case 3:
			//SceneManager::getSceneManager()->switchToScene("Score", this);
			break;
		case 4:
			if (GOManager::GetInstance()->muted == false)
				GOManager::GetInstance()->muted = true;
			else
				GOManager::GetInstance()->muted = false;
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
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}

	// Button checks
	if (GOManager::GetInstance()->muted == false)
	{
		sArray[4] = "Mute";
	}
	if (GOManager::GetInstance()->muted == true)
	{
		sArray[4] = "Unmute";
	}
	int inactive = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (bArray[i]->checkMouse())
			choice = i;
		else
			++inactive;

		if (inactive == 5)
			choice = -1;
	}

	// Physics Simulation Section
	fps = (float)(1.f / dt);
}

void SceneMenu::Render()
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

	//RenderMesh(meshList[GEO_AXES], false);

	// renderButton();

	//On screen text
	RenderTextOnScreen(meshList[GEO_TEXT], "PhysBomber v1.89", Color(1.0f, 1.0f, 1.0f), 2.5f, 2, 56);
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 2, 53);

	RenderTextOnScreen(meshList[GEO_TEXT], "Credits: \n"
		"Ryan Chan \n"
		"Matthew Chan \n"
		"Yan Quan \n"
		"Hui Ling", 
		Color(1, 1, 1), 2.0f, 2, 27);

	RenderTextOnScreen(meshList[GEO_TEXT], 
		"Instructions: \n"
		"A/D - Plane Movement \n"
		"I/K - Tank Movement \n"
		"J/L - Turret Rotation (Tank only) \n"
		"R   - Toggle VTOL     (Harrier only) \n"
		"W/S - Vertical Thrust (Harrier only + VTOL) \n"
		"T   - Flip horizontal (Harrier only + VTOL) \n"
		"C   - Primary Fire    (Planes only) \n"
		"V   - Secondary Fire  (Planes only) \n"
		"N   - Turret Fire     (Tank only) \n", 
		Color(0.3f, 1.0f, 0.3f), 2.0f, 2, 49);

	for (int i = 0; i < 5; ++i)
	{
		// Legacy
		/*
		modelStack.PushMatrix();
		modelStack.Translate(bArray[i]->getPos().x, bArray[i]->getPos().y, bArray[i]->getPos().z);
		modelStack.PushMatrix();
		modelStack.Scale(bArray[i]->getScale().x, bArray[i]->getScale().y, bArray[i]->getScale().z);
		if (bArray[i]->checkMouse())
			meshList[GEO_CUBE]->material.kAmbient.Set(0, 1, 0);
		else
			meshList[GEO_CUBE]->material.kAmbient.Set(1, 0, 0);
		RenderMesh(meshList[GEO_CUBE], true);

		modelStack.PopMatrix();
		modelStack.Scale(4, 4, 4);
		modelStack.Translate(-(float)sArray[i].length() / 4, 0, 0);
		RenderText(meshList[GEO_TEXT], sArray[i], Color(1, 1, 1));
		modelStack.PopMatrix();
		*/

		// Disable highscore button
		if (i != 3)
		{
			RGButtonRender(bArray[i], sArray[i]);
		}
	}

	// Highscore render
	static float X = 146.5f;
	static float Y = 87.0f;
	RenderTextOnScreen(meshList[GEO_TEXT], "HIGH SCORES", Color(0.3f, 0.8f, 1.0f), 3.0f, 65, 55.5f);
	modelStack.PushMatrix();
	modelStack.Translate(X, Y, 0);
	modelStack.Scale(4, 4, 4);
	RenderText(meshList[GEO_TEXT], "PLANE", { 1, 1, 0 });
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	for (size_t i = 0; i < scoresPlane.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(X, Y - 6.0f - i * 5, 0);
		modelStack.Scale(4, 4, 4);
		RenderText(meshList[GEO_TEXT], scoresPlane[i].name, { 1, 1, 1 });
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(X + 10.0f, Y - 6.0f - i * 5, 0);
		modelStack.Scale(4, 4, 4);
		std::string scoreamount = std::to_string(scoresPlane[i].score);
		RenderText(meshList[GEO_TEXT], scoreamount, { 1, 1, 1 });
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(X, Y - 34.0f, 0);
	modelStack.Scale(4, 4, 4);
	RenderText(meshList[GEO_TEXT], "TANK", { 1, 1, 0 });
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	for (size_t i = 0; i < scoresTank.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(X, Y - 40.0f - i * 5, 0);
		modelStack.Scale(4, 4, 4);
		RenderText(meshList[GEO_TEXT], scoresTank[i].name, { 1, 1, 1 });
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(X + 10.0f, Y - 40.0f - i * 5, 0);
		modelStack.Scale(4, 4, 4);
		std::string scoreamount = std::to_string(scoresTank[i].score);
		RenderText(meshList[GEO_TEXT], scoreamount, { 1, 1, 1 });
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
}

void SceneMenu::Exit()
{
	modelStack.Clear();
	viewStack.Clear();
	projectionStack.Clear();

	// Cleanup highscores
	for (size_t i = scoresPlane.size(); i > 0; --i)
	{
		scoresPlane.pop_back();
	}
	for (size_t i = scoresTank.size(); i > 0; --i)
	{
		scoresTank.pop_back();
	}

	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	while (buttonList.size() > 0)
	{
		Button *butt = buttonList.back();//heheh #2
		delete butt;
		buttonList.pop_back();
	}
}
