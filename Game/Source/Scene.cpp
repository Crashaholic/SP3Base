#include "zcustompch.h"
#include "Scene.h"

#include "Application.h"


void Scene::Init()
{	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	
	defaultShader.Init("Shader//comg.vert", "Shader//comg.frag");
	
	// Use our shader
	defaultShader.Use();


	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	defaultShader.SetInt("numLights", (int)0);
	defaultShader.SetBool("textEnabled", (bool)0);

	defaultShader.SetInt  ("lights[0].type"     ,lights[0].type);
	defaultShader.SetVec3 ("lights[0].color"    ,vec3{ lights[0].color.r, lights[0].color.g ,lights[0].color.b });
	defaultShader.SetFloat("lights[0].power"    ,lights[0].power);
	defaultShader.SetFloat("lights[0].kC"       ,lights[0].kC);
	defaultShader.SetFloat("lights[0].kL"       ,lights[0].kL);
	defaultShader.SetFloat("lights[0].kQ"       ,lights[0].kQ);
	defaultShader.SetFloat("lights[0].cosCutoff",lights[0].cosCutoff);
	defaultShader.SetFloat("lights[0].cosInner" ,lights[0].cosInner);
	defaultShader.SetFloat("lights[0].exponent" ,lights[0].exponent);
	defaultShader.SetFloat("transparency" ,1.0f);
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		defaultShader.SetVec3("coloredTexture[" + std::to_string(i) + "]", vec3{ 1.f,1.f,1.f });
		defaultShader.SetVec3("colorableTexture[" + std::to_string(i) + "]", false);
	}
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(0.2f, 0.2f, 0.2f), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.2f, 0.2f, 0.2f), 2.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("Image//Consolas.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_PLAYER_PLANE_A10] = MeshBuilder::GenerateQuad("PLAYER_PLANE_A10", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PLANE_A10]->textureID[0] = LoadTGA("Image//A10.tga");
	meshList[GEO_PLAYER_PLANE_KOMET] = MeshBuilder::GenerateQuad("PLAYER_PLANE_KOMET", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PLANE_KOMET]->textureID[0] = LoadTGA("Image//Komet.tga");
	meshList[GEO_PLAYER_PLANE_HARRIER] = MeshBuilder::GenerateQuad("PLAYER_PLANE_HARRIER", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PLANE_HARRIER]->textureID[0] = LoadTGA("Image//Harrier.tga");
	meshList[GEO_PLAYER_PROJECTILE_MACHINE] = MeshBuilder::GenerateQuad("PLAYER_PROJECTILE_MACHINE", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PROJECTILE_MACHINE]->textureID[0] = LoadTGA("Image//Bullet.tga");
	meshList[GEO_PLAYER_PROJECTILE_MISSILE] = MeshBuilder::GenerateQuad("PLAYER_PROJECTILE_MISSILE", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PROJECTILE_MISSILE]->textureID[0] = LoadTGA("Image//Playermissile.tga");
	meshList[GEO_PLAYER_PROJECTILE_BOMB] = MeshBuilder::GenerateQuad("PLAYER_PROJECTILE_BOMB", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PROJECTILE_BOMB]->textureID[0] = LoadTGA("Image//Bomb1.tga");
	meshList[GEO_PLAYER_PROJECTILE_BOMB]->textureID[1] = LoadTGA("Image//Bomb1decal.tga");
	meshList[GEO_ENEMY_TANK_PASSIVE] = MeshBuilder::GenerateQuad("ENEMY_TANK_PASSIVE", Color(0.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_ENEMY_TANK_PASSIVE]->textureID[0] = LoadTGA("Image//Tank1.tga");
	meshList[GEO_ENEMY_TANK_AGGRESSIVE] = MeshBuilder::GenerateQuad("ENEMY_TANK_AGGRESSIVE", Color(0.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_ENEMY_TANK_AGGRESSIVE]->textureID[0] = LoadTGA("Image//TurretTank1.tga");
	meshList[GEO_PLAYER_TANK] = MeshBuilder::GenerateQuad("PLAYER_TANK_GENERIC", Color(0.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_TANK]->textureID[0] = LoadTGA("Image//TurretTank1.tga");
	meshList[GEO_PLAYER_TANKGUN] = MeshBuilder::GenerateQuad("PLAYER_TANKGUN_GENERIC", Color(0.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_TANKGUN]->textureID[0] = LoadTGA("Image//Turret1.tga");
	meshList[GEO_PLAYER_PROJECTILE_SHELL] = MeshBuilder::GenerateSphere("PLAYER_PROJECTILE_SHELL", Color(1.0f, 0.0f, 1.0f), 10, 10, 1.f);
	meshList[GEO_EXPLOSION] = MeshBuilder::GenerateQuad("EXPLOSION", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_EXPLOSION]->textureID[0] = LoadTGA("Image//Explosion.tga");
	meshList[GEO_ENEMY_BUILDING] = MeshBuilder::GenerateQuad("ENEMY_BUILDING", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_ENEMY_BUILDING]->textureID[0] = LoadTGA("Image//Enemybuilding1.tga");
	meshList[GEO_ENEMY_PLANE_PASSIVE] = MeshBuilder::GenerateQuad("ENEMY_PLANE_PASSIVE", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_ENEMY_PLANE_PASSIVE]->textureID[0] = LoadTGA("Image//Antonov.tga");
	meshList[GEO_DEBUG] = MeshBuilder::GenerateSphere("DEBUG", Color(1.0f, 0.5f, 0.5f), 10, 10, 1.f);

	meshList[GEO_UPGRADE_1] = MeshBuilder::GenerateQuad("upgrade1", Color(0.5f, 0.f, 0.5f), 2.0f);
	meshList[GEO_UPGRADE_1]->textureID[0] = LoadTGA("Image//upgrade_1.tga");
	meshList[GEO_UPGRADE_2] = MeshBuilder::GenerateQuad("upgrade2", Color(0.5f, 0.5f, 0.5f), 2.0f);
	meshList[GEO_UPGRADE_2]->textureID[0] = LoadTGA("Image//upgrade_2.tga");
	meshList[GEO_UPGRADE_3] = MeshBuilder::GenerateQuad("upgrade3", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_UPGRADE_3]->textureID[0] = LoadTGA("Image//upgrade_3.tga");
	meshList[GEO_GRADIENTR] = MeshBuilder::GenerateGradient("grad1", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_GRADIENTG] = MeshBuilder::GenerateGradient("grad2", Color(0.f, 1.f, 0.f), 2.0f);
	meshList[GEO_GRADIENTB] = MeshBuilder::GenerateGradient("grad3", Color(0.f, 0.f, 1.f), 2.0f);

	meshList[GEO_UPGRADE_1_HUD] = MeshBuilder::GenerateQuad("upgrade1", Color(0.5f, 0.f, 0.5f), 2.0f);
	meshList[GEO_UPGRADE_1_HUD]->textureID[0] = LoadTGA("Image//upgrade_1_HUD.tga");
	meshList[GEO_UPGRADE_2_HUD] = MeshBuilder::GenerateQuad("upgrade2", Color(0.5f, 0.5f, 0.5f), 2.0f);
	meshList[GEO_UPGRADE_2_HUD]->textureID[0] = LoadTGA("Image//upgrade_2_HUD.tga");
	meshList[GEO_UPGRADE_3_HUD] = MeshBuilder::GenerateQuad("upgrade3", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_UPGRADE_3_HUD]->textureID[0] = LoadTGA("Image//upgrade_3_HUD.tga");

	meshList[GEO_KILL] = MeshBuilder::GenerateQuad("kill", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_KILL]->textureID[0] = LoadTGA("Image//kill.tga");

	meshList[GEO_ARROW] = MeshBuilder::GenerateQuad("arrow", Color(1.f, 0.f, 0.f), 2.0f);
	meshList[GEO_ARROW]->textureID[0] = LoadTGA("Image//arrow.tga");

	meshList[GEO_RAIN] = MeshBuilder::GenerateCube("rain", Color(0.5f, 0.5f, 1.0f), 2.f);

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();

	rainRandom();
	if (GOManager::GetInstance()->rain == true)
	{
		// Push back number of rain objects according to rain intensity
		for (int i = 0; i < GOManager::GetInstance()->RAIN_INTENSITY; ++i)
		{
			GameObject *a = GOManager::GetInstance()->fetchGO();
			a->scale.Set(4.0f, 0.2f, 1.0f);
			a->type = GameObject::GO_RAIN;
			a->wrapMode = GameObject::SW_NONE;
			a->angle = -90.0f;
			a->norm.Set(cos(Math::DegreeToRadian(a->angle)), sin(Math::DegreeToRadian(a->angle)), 0.0f);
			a->pos.Set(
				Math::RandFloatMinMax(-150.0f, 126.0f),
				Math::RandFloatMinMax(100.0f + a->scale.y, 270.0f),
				0.0f);

			if (GOManager::GetInstance()->wind)
			{
				a->vel = GOManager::GetInstance()->windVector * GOManager::GetInstance()->WIND_POWER + GOManager::GetInstance()->gravity;
			}
			else
			{
				a->vel = GOManager::GetInstance()->gravity;
			}

			a->hasGravity = true;
			a->reserved = true;
		}
	}
}

void Scene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	//glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	//glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	//glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	//glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	//glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	defaultShader.SetBool("textEnabled", true);
	defaultShader.SetBool("lightEnabled", false);
	defaultShader.SetBool("colorTextureEnabled[0]", true);
	defaultShader.SetVec3("textColor", { color.r,color.g,color.b });
	defaultShader.SetInt("colorTexture[0]", 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		//1.0f is the spacing of each character, you may change this value
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		defaultShader.SetMat4("model", MVP);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	defaultShader.SetBool("textEnabled", false);
	glEnable(GL_DEPTH_TEST);
}

void Scene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	//glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	defaultShader.SetBool("textEnabled", true);
	defaultShader.SetBool("lightEnabled", false);
	defaultShader.SetBool("colorTextureEnabled[0]", true);
	defaultShader.SetVec3("textColor", { color.r,color.g,color.b });
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	defaultShader.SetInt("colorTexture[0]", 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		//1.0f is the spacing of each character, you may change this value
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		defaultShader.SetMat4("model", MVP);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	defaultShader.SetBool("textEnabled", false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Scene::RenderMesh(Mesh *mesh, bool enableLight)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	defaultShader.SetMat4("model", MVP);
	if (enableLight && bLightEnabled)
	{
		defaultShader.SetBool("lightEnabled", true);
		modelView = viewStack.Top() * modelStack.Top();
		defaultShader.SetMat4("view", modelView);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		defaultShader.SetMat4("proj", modelView_inverse_transpose);

		//load material
		defaultShader.SetVec3("material.kAmbient", { mesh->material.kAmbient.r, mesh->material.kAmbient.g, mesh->material.kAmbient.b, });
		defaultShader.SetVec3("material.kDiffuse", { mesh->material.kDiffuse.r, mesh->material.kDiffuse.g, mesh->material.kDiffuse.b });
		defaultShader.SetVec3("material.kSpecular", { mesh->material.kSpecular.r, mesh->material.kSpecular.g , mesh->material.kSpecular.b });
		defaultShader.SetFloat("material.kShininess", mesh->material.kShininess);
	}
	else
	{
		defaultShader.SetBool("lightEnabled", false);
	}
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureID[i] > 0)
		{
			defaultShader.SetBool("colorTextureEnabled[" + std::to_string(i) + "]", true);
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			defaultShader.SetInt("colorTexture[" + std::to_string(i) + "]", i);
		}
		else
		{
			defaultShader.SetBool("colorTextureEnabled[" + std::to_string(i) + "]", false);
		}
	}
	mesh->Render();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureID[i] > 0)
			glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Scene::RenderGO(GameObject *go)
{
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		defaultShader.SetVec3("coloredTexture[" + std::to_string(i) + "]", vec3{ go->color[i].r,go->color[i].g,go->color[i].b });
		//defaultShader.SetVec3("colorableTexture[" + std::to_string(i) + "]", go->isColorable[i]);
	}

	glDisable(GL_CULL_FACE);
	if (go->Iframes <= 0.0)
		defaultShader.SetFloat("transparency", go->transparency);
	else
		defaultShader.SetFloat("transparency", 0.5f);
	int renders = 1;
	if (go->wrapMode == GameObject::SW_HYBRID)
	{
		renders = 3;
	}
	for (int i = 0; i < renders; ++i)
	{
		modelStack.PushMatrix();
		switch(i)
		{
		case 0:
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			break;
		case 1:
			modelStack.Translate(go->pos.x + m_worldWidth, go->pos.y, go->pos.z);
			break;
		case 2:
			modelStack.Translate(go->pos.x - m_worldWidth, go->pos.y, go->pos.z);
			break;
		}
		modelStack.Rotate(Math::RadianToDegree(atan2(go->norm.y, go->norm.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		switch (go->type)
		{
		case GameObject::GO_BALL:
			RenderMesh(meshList[GEO_BALL], false);
			break;
		case GameObject::GO_CUBE:
			RenderMesh(meshList[GEO_CUBE], false);
			break;
		case GameObject::ENEMY_BUILDING:
			RenderMesh(meshList[GEO_ENEMY_BUILDING], false);
			break;
		case GameObject::PLAYER_PLANE_A10:
			RenderMesh(meshList[GEO_PLAYER_PLANE_A10], false);
			break;
		case GameObject::ENEMY_PLANE_PASSIVE:
			RenderMesh(meshList[GEO_ENEMY_PLANE_PASSIVE], false);
			break;
		case GameObject::ENEMY_PLANE_AGGRESSIVE:
		case GameObject::PLAYER_PLANE_KOMET:
			RenderMesh(meshList[GEO_PLAYER_PLANE_KOMET], false);
			break;
		case GameObject::PLAYER_PLANE_HARRIER:
			RenderMesh(meshList[GEO_PLAYER_PLANE_HARRIER], false);
			break;
		case GameObject::PLAYER_TANK:
			RenderMesh(meshList[GEO_PLAYER_TANK], false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_PLAYER_TANKGUN], false);
			break;
		case GameObject::ENEMY_TANK_PASSIVE:
			RenderMesh(meshList[GEO_ENEMY_TANK_PASSIVE], false);
			break;

		case GameObject::ENEMY_TANK_AGGRESSIVE:
			RenderMesh(meshList[GEO_ENEMY_TANK_AGGRESSIVE], false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_PLAYER_TANKGUN], false);
			break;
		case GameObject::PLAYER_PROJECTILE_MACHINE:
		case GameObject::PLAYER_PROJECTILE_SHELL:
		case GameObject::ENEMY_PROJECTILE_MACHINE:
			RenderMesh(meshList[GEO_PLAYER_PROJECTILE_MACHINE], false);
			break;
		case GameObject::ENEMY_PROJECTILE_BOMB:
		case GameObject::PLAYER_PROJECTILE_BOMB:
		case GameObject::PLAYER_PROJECTILE_NUKE:
			RenderMesh(meshList[GEO_PLAYER_PROJECTILE_BOMB], false);
			break;
		case GameObject::EXPLOSION:
			RenderMesh(meshList[GEO_EXPLOSION], false);
			break;
		case GameObject::UPGRADE_1:
			RenderMesh(meshList[GEO_UPGRADE_1], false);
			break;
		case GameObject::UPGRADE_2:
			RenderMesh(meshList[GEO_UPGRADE_2], false);
			break;
		case GameObject::UPGRADE_3:
			RenderMesh(meshList[GEO_UPGRADE_3], false);
			break;
		case GameObject::PLAYER_PROJECTILE_MISSILE:
			RenderMesh(meshList[GEO_PLAYER_PROJECTILE_MISSILE], false);
			break;
		case GameObject::GO_RAIN:
			RenderMesh(meshList[GEO_RAIN], false);
			break;
		}
		modelStack.PopMatrix();
	}
	
	if (go->hasCollider() && false)
		debugBalls(go);

	glEnable(GL_CULL_FACE);
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		defaultShader.SetVec3("coloredTexture[" + std::to_string(i) + "]", vec3{ 1.f,1.f,1.f });
		//defaultShader.SetVec3("colorableTexture[" + std::to_string(i) + "]", go->isColorable[i]);
	}
	defaultShader.SetFloat("transparency", 1.0f);
}

void Scene::checkSwitch()
{
	if (Application::IsKeyPressed('5'))
	{
		SceneManager::getSceneManager().switchToScene("Menu", this);
	}
	if (Application::IsKeyPressed('6'))
	{
		SceneManager::getSceneManager().switchToScene("Plane", this);
	}
	if (Application::IsKeyPressed('7'))
	{
		SceneManager::getSceneManager().switchToScene("Tank", this);
	}
	if (Application::IsKeyPressed('8'))
	{
		SceneManager::getSceneManager().switchToScene("PSelect", this);
	}
	if (Application::IsKeyPressed('9'))
	{
		SceneManager::getSceneManager().switchToScene("TSelect", this);
	}
	if (Application::IsKeyPressed('0'))
	{
		SceneManager::getSceneManager().switchToScene("Score", this);
	}
	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		SceneManager::getSceneManager().switchToScene("End", this);
	}
	if (Application::IsKeyPressed(VK_OEM_4))
	{
		SceneManager::getSceneManager().switchToScene("2Select", this);
	}
	if (Application::IsKeyPressed(VK_OEM_6))
	{
		SceneManager::getSceneManager().switchToScene("2P", this);
	}
}

void Scene::debugBalls(GameObject * go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->corn[0].x, go->corn[0].y, go->corn[0].z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_DEBUG], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(go->corn[1].x, go->corn[1].y, go->corn[1].z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_DEBUG], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(go->corn[2].x, go->corn[2].y, go->corn[2].z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_DEBUG], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(go->corn[3].x, go->corn[3].y, go->corn[3].z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_DEBUG], false);
	modelStack.PopMatrix();
}

void Scene::addButton(Button * button)
{
	buttonList.push_back(button);
}

void Scene::cleanButton()
{
	while (buttonList.size() > 0)
	{
		Button *button = buttonList.back();
		delete button;
		buttonList.pop_back();
	}
}

void Scene::renderButton()
{
	for (std::vector<Button *>::iterator it = buttonList.begin(); it != buttonList.end(); ++it)
	{
		Button *button = *it;
		modelStack.PushMatrix();
		modelStack.Translate(button->balls[0].x, button->balls[0].y, button->balls[0].z);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_DEBUG], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(button->balls[1].x, button->balls[1].y, button->balls[1].z);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_DEBUG], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(button->balls[2].x, button->balls[2].y, button->balls[2].z);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_DEBUG], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(button->balls[3].x, button->balls[3].y, button->balls[3].z);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_DEBUG], false);
		modelStack.PopMatrix();
	}
}

void Scene::render1PHUD()
{
	modelStack.PushMatrix();
	modelStack.Translate(5.0f, 95.0f, 0);
	modelStack.Scale(4.0f, 4.0f, 1.0f);
	RenderMesh(meshList[GEO_UPGRADE_3_HUD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.0f + HUD_SPACING, 95.0f, 0);
	modelStack.Scale(4.0f, 4.0f, 1.0f);
	RenderMesh(meshList[GEO_UPGRADE_1_HUD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.0f + HUD_SPACING * 2.0f, 95.0f, 0);
	modelStack.Scale(4.0f, 4.0f, 1.0f);
	RenderMesh(meshList[GEO_UPGRADE_2_HUD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.0f + HUD_SPACING * 3.0f, 95.0f, 0);
	modelStack.Scale(4.0f, 4.0f, 1.0f);
	RenderMesh(meshList[GEO_KILL], false);
	modelStack.PopMatrix();
}

void Scene::render2PHUD()
{
	modelStack.PushMatrix();
	modelStack.Translate(5.0f, 95.0f, 0);
	modelStack.Scale(4.0f, 4.0f, 1.0f);
	RenderMesh(meshList[GEO_UPGRADE_3_HUD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.0f, 87.0f, 0);
	modelStack.Scale(4.0f, 4.0f, 1.0f);
	RenderMesh(meshList[GEO_UPGRADE_3_HUD], false);
	modelStack.PopMatrix();
}

void Scene::cleanVar()
{
	// GOManager clean variables
	GOManager::GetInstance()->planeLives = 2;
	GOManager::GetInstance()->tankLives = 2;
	GOManager::GetInstance()->upgrade_1 = 0;
	GOManager::GetInstance()->upgrade_2 = 0;
	GOManager::GetInstance()->attackCount = 0;
	GOManager::GetInstance()->planeKills = 0;
	GOManager::GetInstance()->tankKills = 0;
	GOManager::GetInstance()->planeAccuracy = 0.0f;
	GOManager::GetInstance()->tankAccuracy = 0.0f;
	GOManager::GetInstance()->planeHighscore = 0;
	GOManager::GetInstance()->tankHighscore = 0;
	GOManager::GetInstance()->totalHits = 0;
	GOManager::GetInstance()->totalShots = 0;
	GOManager::GetInstance()->windBT = GOManager::GetInstance()->WIND_TIMER;
	GOManager::GetInstance()->windVector = Vector3(1, 0, 0);
	GOManager::GetInstance()->windVectorN = Vector3(1, 0, 0);
	GOManager::GetInstance()->rain = false;
	GOManager::GetInstance()->wind = true;
}

void Scene::RGButtonRender(Button * b, std::string s)
{
	modelStack.PushMatrix();
	modelStack.Translate(b->getPos().x, b->getPos().y, b->getPos().z);

		modelStack.PushMatrix();
		modelStack.Scale(b->getScale().x, b->getScale().y, b->getScale().z);
		if (b->checkMouse())
			meshList[GEO_CUBE]->material.kAmbient.Set(0, 1, 0);
		else
			meshList[GEO_CUBE]->material.kAmbient.Set(1, 0, 0);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();

	modelStack.Scale(4, 4, 4);
	modelStack.Translate((-(float)s.length() / 4) + 0.5f, 0, 0);
	RenderText(meshList[GEO_TEXT], s, Color(1, 1, 1));
	modelStack.PopMatrix();
}

void Scene::windRender()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2.0f, 90.0f, 0.0f);

	modelStack.Rotate(
		Math::RadianToDegree(atan2(
				GOManager::GetInstance()->windVector.y, 
				GOManager::GetInstance()->windVector.x)), 0, 0, 1);

	modelStack.Scale(8.0f, 8.0f, 8.0f);
	RenderMesh(meshList[GEO_ARROW], false);
	modelStack.PopMatrix();
}

void Scene::rainRandom()
{
	if (Math::RandFloatMinMax(0.0f, 100.0f) < GOManager::GetInstance()->RAIN_CHANCE)
		GOManager::GetInstance()->rain = true;
	else
		GOManager::GetInstance()->rain = false;
}

void Scene::goWrap()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			// unspawn bullets when they leave screen
			bool hit = false;
			switch (go->wrapMode)
			{
			case GameObject::SW_OFFSCREENCLEAR:
				if (go->pos.x > m_worldWidth + 5.0f
					|| go->pos.x < -5.0f || go->pos.y < -5.0f)
					go->active = false;
				hit = true;
				break;
			case GameObject::SW_CLAMP:
				go->pos.x = Math::Clamp(go->pos.x, go->scale.x, m_worldWidth - go->scale.x);
				go->pos.y = Math::Clamp(go->pos.y, go->scale.y, m_worldHeight - go->scale.y);
				hit = true;
				break;
			case GameObject::SW_CLEAR:
				if (go->pos.x > m_worldWidth || go->pos.y > m_worldHeight*2
					|| go->pos.x < 0 || go->pos.y < 0)
					go->active = false;
				hit = true;
				break;
			case GameObject::SW_BOUNCE:
				if (go->pos.x > m_worldWidth)
				{
					go->vel.x *= -1.0f;
					go->dir.x *= -1.0f;
					go->pos.x = m_worldWidth;
					hit = true;
				}
				if (go->pos.y > m_worldHeight)
				{
					go->vel.y *= -1.0f;
					go->dir.y *= -1.0f;
					go->pos.y = m_worldHeight;
					hit = true;
				}
				if (go->pos.x < 0)
				{
					go->vel.x *= -1.0f;
					go->dir.x *= -1.0f;
					go->pos.x = 0;
					hit = true;
				}
				if (go->pos.y < 0)
				{
					go->vel.y *= -1.0f;
					go->dir.y *= -1.0f;
					go->pos.y = 0;
					hit = true;
				}
				if (hit)
					go->angle = (atan2(go->dir.y, go->dir.x));
				break;
			case GameObject::SW_WRAP:
				if (go->pos.x > m_worldWidth)
					go->pos.x = 0;
				if (go->pos.x < 0)
					go->pos.x = m_worldWidth - 0.1f;

				if (go->pos.y > m_worldHeight)
					go->pos.y = 0;
				if (go->pos.y < 0)
					go->pos.y = m_worldHeight - 0.1f;
			case GameObject::SW_HYBRID:
				if (go->pos.x > m_worldWidth)
					go->pos.x = 0;
				if (go->pos.x < 0)
					go->pos.x = m_worldWidth - 0.1f;
				if (go->pos.y > m_worldHeight)
				{
					go->vel.y *= -1.0f;
					go->dir.y *= -1.0f;
					go->pos.y = m_worldHeight;
					hit = true;
				}
				if (go->pos.y < 0)
				{
					go->vel.y *= -1.0f;
					go->dir.y *= -1.0f;
					go->pos.y = 0;
					hit = true;
				}
				if (hit)
					go->angle = /*Math::RadianToDegree*/(atan2(go->vel.y, go->vel.x));
				break;
			case GameObject::SW_NONE:
				break;
			}
		}
	}
}