#include "Terrain.h"
#include "MyMath.h"
#include "Logging.h"
#include <GL\glew.h>

Terrain::Terrain()
	: tMesh(nullptr)
{
}

Terrain::~Terrain()
{
	if (tMesh)
	{
		delete tMesh;
		tMesh = nullptr;
	}
}

Vector3 Terrain::GetHeight(Vector3 Position)
{
	if (Position.x > Points[TERRAIN_SIZE - 1].x) // checking for outside of screen on the right side
	{
		float newY;
		newY = Points[TERRAIN_SIZE - 1].y;
		//newY = Points[TERRAIN_SIZE - 2].y +
		//	(((Points[TERRAIN_SIZE - 1].x - 1.f / TERRAIN_SIZE) - Points[TERRAIN_SIZE - 2].x) /
		//	(Points[TERRAIN_SIZE - 1].x - Points[TERRAIN_SIZE - 2].x)) 
		//	* (Points[TERRAIN_SIZE - 1].y - Points[TERRAIN_SIZE - 2].y);
		return vec3(Position.x, newY);
	}
	else if (Position.x < Points[0].x) // checking for outside of screen on the left side
	{
		float newY;
		newY = Points[0].y;
		//newY = Points[0].y + ((Position.x - Points[0].x) / (Points[1].x - Points[0].x)) * (Points[1].y - Points[0].y);
		return vec3(Position.x, newY);
	}
	vec3 R, L;
	for (int i = 1; i < TERRAIN_SIZE; ++i)
	{
		if (Points[i].x > Position.x)
		{
			R = Points[i];
			L = Points[i - 1];
			break;
		}
	}
	float newY;
	newY = L.y + ((Position.x - L.x) / (R.x - L.x)) * (R.y - L.y);
	return Vector3(Position.x, newY);
}

Vector3 Terrain::GetNormal(Vector3 Position)
{
	if (Position.x > Points[TERRAIN_SIZE - 1].x) // checking for outside of screen on the right side
	{
		vec3 v = (Points[TERRAIN_SIZE - 2] - Points[TERRAIN_SIZE - 1]).Normalized();
		return vec3(v.y, -v.x, 0);
	}
	else if (Position.x < Points[0].x) // checking for outside of screen on the left side
	{
		vec3 v = (Points[0] - Points[1]).Normalized();
		return vec3(v.y, -v.x, 0);
	}
	vec3 p1, p2;
	for (int i = 1; i < TERRAIN_SIZE; ++i)
	{
		if (Points[i].x > Position.x)
		{
			p1 = Points[i];
			p2 = Points[i - 1];
			break;
		}
	}
	vec3 v = (p2 - p1).Normalized();
	return vec3(v.y, -v.x, 0);
}

void Terrain::Update(double dt)
{
	float transitionSpeed = 10.0f;
	bool terrainDifference = false;
	for (unsigned int i = 0; i < TERRAIN_SIZE;++i)
	{
		Points[i].x = NewPoints[i].x;
		if (Points[i].y != NewPoints[i].y)
		{
			if (fabs(NewPoints[i].y - Points[i].y) <= transitionSpeed * static_cast<float>(dt))
			{
				Points[i].y = NewPoints[i].y;
			}
			else
			{
				if (Points[i].y < NewPoints[i].y)
					Points[i].y += transitionSpeed * static_cast<float>(dt);
				else
					Points[i].y -= transitionSpeed * static_cast<float>(dt);
			}
			terrainDifference = true;
		}
	}
	if (terrainDifference)
		GenerateTerrainMesh();
}

void Terrain::GenerateRandomHeight(float worldWidth)
{
	float lastY = 0;
	for (int i = 0; i < TERRAIN_SIZE; ++i)
	{

		if (i == 0)
		{
			NewPoints[i].y = Math::RandFloatMinMax(10.f, 20.f);
			lastY = NewPoints[i].y;
			NewPoints[i].x = 0;
			Bottom[i].x = 0;
		}
		else if (i == TERRAIN_SIZE - 1)
		{
			NewPoints[i].x = static_cast<float>(worldWidth);
			Bottom[i].x = static_cast<float>(worldWidth);
			NewPoints[i].y = lastY + Math::RandFloatMinMax((float)TERRAIN_DN, (float)TERRAIN_UP);
		}
		else
		{
			NewPoints[i].x = i * ((float)worldWidth / ((float)TERRAIN_SIZE - 1.f));
			Bottom[i].x = i * ((float)worldWidth / ((float)TERRAIN_SIZE - 1.f));
			NewPoints[i].y = lastY + Math::RandFloatMinMax((float)TERRAIN_DN, (float)TERRAIN_UP);
			if (NewPoints[i].y < 0)
			{
				lastY = 3;
				NewPoints[i].y = 3;
			}
			lastY = NewPoints[i].y;
		}
		Points[i].x = NewPoints[i].x;

		//LOG_NONE("Points[%]: %", i, Points[i]);
	}

}

void Terrain::GenerateTerrainMesh()
{
	Vertex v;
	std::vector<GLuint> index_buffer_data;
	v.pos.Set(0, 0, 0);
	v.color.Set(1, 1, 1);
	std::vector<Vertex> vertex_buffer_data;
	vertex_buffer_data.push_back(v);
	index_buffer_data.push_back(0);
	for (int i = 0; i < TERRAIN_SIZE; ++i)
	{
		v.pos.Set(Points[i].x, Points[i].y, Points[i].z);
		v.color.Set(0.3f, 0.3f, 0.3f);
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (int i = 0; i < TERRAIN_SIZE; ++i)
	{
		v.pos.Set(Bottom[i].x, Bottom[i].y, Bottom[i].z);
		v.color.Set(0.4f, 0.4f, 0.4f);
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (int i = 0; i < TERRAIN_SIZE; ++i)
	{
		index_buffer_data.push_back(i + TERRAIN_SIZE + 1);
		index_buffer_data.push_back(i+ 1);
	}

	if (!tMesh) // Ensure we always have one mesh
		tMesh = new Mesh("2DTerrain");
	tMesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, tMesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tMesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	tMesh->indexSize = index_buffer_data.size();
}

void Terrain::DeformTerrain(Vector3 ExplosionPosition, float ExplosionRadius)
{
	for (unsigned int i = 0; i < TERRAIN_SIZE; ++i)
	{
		if ((Points[i] - ExplosionPosition).Length() > ExplosionRadius*2.0f)
			continue;
		NewPoints[i].y -= ExplosionRadius / Math::Max(1.0f,(Points[i] - ExplosionPosition).Length());
		NewPoints[i].y = Math::Max(NewPoints[i].y, 1.0f);
		Points[i].y -= ExplosionRadius / Math::Max(1.0f,(Points[i] - ExplosionPosition).Length());
		Points[i].y = Math::Max(Points[i].y, 1.0f);
	}
	GenerateTerrainMesh();
}