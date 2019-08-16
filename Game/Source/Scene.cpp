#include "Scene.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>


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
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("Image//pico8.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_PLAYER_PLANE_A10] = MeshBuilder::GenerateQuad("PLAYER_PLANE_A10", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PLANE_A10]->textureID[0] = LoadTGA("Image//A10.tga");
	meshList[GEO_PLAYER_PLANE_KOMET] = MeshBuilder::GenerateQuad("PLAYER_PLANE_KOMET", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PLANE_KOMET]->textureID[0] = LoadTGA("Image//Komet.tga");
	meshList[GEO_PLAYER_PROJECTILE_MACHINE] = MeshBuilder::GenerateQuad("PLAYER_PROJECTILE_MACHINE", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PROJECTILE_MACHINE]->textureID[0] = LoadTGA("Image//Bomb1.tga");
	meshList[GEO_PLAYER_PROJECTILE_BOMB] = MeshBuilder::GenerateQuad("PLAYER_PROJECTILE_BOMB", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PROJECTILE_BOMB]->textureID[0] = LoadTGA("Image//Bomb1.tga");
	meshList[GEO_PLAYER_PROJECTILE_BOMB]->textureID[1] = LoadTGA("Image//Bomb1decal.tga");
	meshList[GEO_PLAYER_TANK] = MeshBuilder::GenerateQuad("PLAYER_TANK_GENERIC", Color(0.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_TANKGUN] = MeshBuilder::GenerateQuad("PLAYER_TANKGUN_GENERIC", Color(0.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_PLAYER_PROJECTILE_SHELL] = MeshBuilder::GenerateSphere("PLAYER_PROJECTILE_SHELL", Color(1.0f, 0.0f, 1.0f), 10, 10, 1.f);
	meshList[GEO_EXPLOSION] = MeshBuilder::GenerateQuad("EXPLOSION", Color(1.0f, 1.0f, 1.0f), 2.0f);
	meshList[GEO_EXPLOSION]->textureID[0] = LoadTGA("Image//Explosion.tga");
	meshList[GEO_DEBUG] = MeshBuilder::GenerateSphere("DEBUG", Color(1.0f, 0.5f, 0.5f), 10, 10, 1.f);
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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
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
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0);
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

	defaultShader.SetFloat("transparency", go->transparency);

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
		case GameObject::PLAYER_PLANE_A10:
			RenderMesh(meshList[GEO_PLAYER_PLANE_A10], false);
			break;
		case GameObject::PLAYER_PLANE_KOMET:
			RenderMesh(meshList[GEO_PLAYER_PLANE_KOMET], false);
			break;
		case GameObject::PLAYER_TANK:
			RenderMesh(meshList[GEO_PLAYER_TANK], false);
			break;
		case GameObject::PLAYER_TANKGUN:
			RenderMesh(meshList[GEO_PLAYER_TANKGUN], false);
			break;
		case GameObject::PLAYER_PROJECTILE_MACHINE:
			RenderMesh(meshList[GEO_PLAYER_PROJECTILE_MACHINE], false);
			break;
		case GameObject::PLAYER_PROJECTILE_BOMB:
			RenderMesh(meshList[GEO_PLAYER_PROJECTILE_BOMB], false);
			break;
		case GameObject::PLAYER_PROJECTILE_SHELL:
			RenderMesh(meshList[GEO_PLAYER_PROJECTILE_SHELL], false);
			break;
		case GameObject::EXPLOSION:
			RenderMesh(meshList[GEO_EXPLOSION], false);
			break;
		}
		modelStack.PopMatrix();
	}
	
	if (go->hasCollider())
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