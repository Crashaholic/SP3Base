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

Vector3 Terrain::GetNormal(Vector3 ObjectPosition)
{
	vec3 p1, p2;
	for (int i = 1; i < TERRAIN_SIZE; ++i)
	{
		if (Points[i].x > ObjectPosition.x)
		{
			p1 = Points[i];
			p2 = Points[i - 1];
			break;
		}
	}
	vec3 v = (p2 - p1).Normalized();
	return vec3(v.y, -v.x, 0);
}

void Terrain::GenerateRandomHeight(float worldWidth)
{
	float lastY = 0;
	for (int i = 0; i < TERRAIN_SIZE; ++i)
	{

		if (i == 0)
		{
			Points[i].y = Math::RandFloatMinMax(10.f, 20.f);
			lastY = Points[i].y;
			Points[i].x = 0;
			Bottom[i].x = 0;
		}
		else if (i == TERRAIN_SIZE - 1)
		{
			Points[i].x = static_cast<float>(worldWidth);
			Bottom[i].x = static_cast<float>(worldWidth);
			Points[i].y = lastY + Math::RandFloatMinMax((float)TERRAIN_DN, (float)TERRAIN_UP);
		}
		else
		{
			Points[i].x = i * ((float)worldWidth / ((float)TERRAIN_SIZE - 1.f));
			Bottom[i].x = i * ((float)worldWidth / ((float)TERRAIN_SIZE - 1.f));
			Points[i].y = lastY + Math::RandFloatMinMax((float)TERRAIN_DN, (float)TERRAIN_UP);
			if (Points[i].y < 0)
			{
				lastY = 3;
				Points[i].y = 3;
			}
			lastY = Points[i].y;
		}
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
		Points[i].y -= ExplosionRadius / Math::Max(1.0f,(Points[i] - ExplosionPosition).Length());
		Points[i].y = Math::Max(Points[i].y, 1.0f);
	}
	GenerateTerrainMesh();
}

Vector3 Terrain::GetHeight(Vector3 TankPos)
{
	vec3 R, L;
	for (int i = 1; i < TERRAIN_SIZE; ++i)
	{
		if (Points[i].x > TankPos.x)
		{
			R = Points[i];
			L = Points[i - 1];
			break;
		}
	}
	float newY;
	newY = L.y + ((TankPos.x - L.x) / (R.x - L.x)) * (R.y - L.y);
	return Vector3(TankPos.x, newY);
}