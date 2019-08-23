#include "zcustompch.h"
#include "SceneScore.h"

#include "Application.h"

#include "SceneManager.h"

SceneScore::SceneScore()
{
}

SceneScore::~SceneScore()
{
}

void SceneScore::Init()
{
	Scene::Init();
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);


	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	backButt.init({ m_worldWidth / 2.f - 69.0f, m_worldHeight - 3.5f }, { 20.0f, 3.5f, 1.0f });

	bLightEnabled = true;
	Math::InitRNG();

	HighScoreSystem::GetInstance()->ParseFile(0, &scoresPlane);
	HighScoreSystem::GetInstance()->ParseFile(1, &scoresTank);
}

void SceneScore::Update(double dt)
{

#ifdef _DEBUG
	//Keyboard Section
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

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
		if (backButt.checkMouse())
		{
			SceneManager::getSceneManager()->switchToScene("Menu", this);
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

	// Physics Simulation Section
	fps = (float)(1.f / dt);
}

void SceneScore::Render()
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
		modelStack.Translate(m_worldWidth / 2.f - 47.f, m_worldHeight / 2.f + 20.f, 0);
		modelStack.Scale(7, 7, 7);
		RenderText(meshList[GEO_TEXT], "PLANE", { 1, 1, 1 });
		modelStack.PopMatrix();

	modelStack.PushMatrix();
	for (size_t i = 0; i < scoresPlane.size(); ++i)
	{
		modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.f - 50.f, m_worldHeight / 2.f - i * 5 + 10.f, 0);
			modelStack.Scale(5, 5, 5);
			RenderText(meshList[GEO_TEXT], scoresPlane[i].name, { 1, 1, 1 });
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.f - 40.f, m_worldHeight / 2.f - i * 5 + 10.f, 0);
			modelStack.Scale(5, 5, 5);
			std::string scoreamount = std::to_string(scoresPlane[i].score);
			RenderText(meshList[GEO_TEXT], scoreamount, { 1, 1, 1 });
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2.f + 47.f, m_worldHeight / 2.f + 20.f, 0);
	modelStack.Scale(7, 7, 7);
	RenderText(meshList[GEO_TEXT], "TANK", { 1, 1, 1 });
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	for (size_t i = 0; i < scoresTank.size(); ++i)
	{
		modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.f + 40.f, m_worldHeight / 2.f - i * 5 + 10.f, 0);
			modelStack.Scale(5, 5, 5);
			RenderText(meshList[GEO_TEXT], scoresTank[i].name, { 1, 1, 1 });
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.f + 50.f, m_worldHeight / 2.f - i * 5 + 10.f, 0);
			modelStack.Scale(5, 5, 5);
			std::string scoreamount = std::to_string(scoresTank[i].score);
			RenderText(meshList[GEO_TEXT], scoreamount, { 1, 1, 1 });
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	RGButtonRender(&backButt, "Back");

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);
}

void SceneScore::Exit()
{
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
}
