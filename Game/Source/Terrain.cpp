#include "Terrain.h"
#include "MyMath.h"
#include "Logging.h"
#include <vector>
#include <GL\glew.h>

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::GenerateRandomHeight(unsigned int worldWidth)
{
	for (int i = 0; i < 64; ++i)
	{
		Points[i].y = Math::RandFloatMinMax(2.f, 20.f);
		if (i == 0)
		{
			Points[i].x = 0;
			Bottom[i].x = 0;
		}
		else if (i == 63)
		{
			Points[i].x = worldWidth;
			Bottom[i].x = worldWidth;
		}
		else
		{
			Points[i].x = i * ((float)worldWidth / 63.f);
			Bottom[i].x = i * ((float)worldWidth / 63.f);
		}
		LOG_NONE("Points[%]: %", i, Points[i]);
	}
}

void Terrain::GenerateTerrainMesh()
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.pos.Set(0, 0, 0);
	v.color.Set(1, 1, 1);
	vertex_buffer_data.push_back(v);
	index_buffer_data.push_back(0);
	for (int i = 0; i < 64; ++i)
	{
		v.pos.Set(Bottom[i].x, Bottom[i].y, Bottom[i].z);
		v.color.Set(1, 1, 1);
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (int i = 0; i < 64; ++i)
	{
		v.pos.Set(Points[i].x, Points[i].y, Points[i].z);
		v.color.Set(1, 1, 1);
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (int i = 0; i < 32; ++i)
	{
		index_buffer_data.push_back(i + 1);
		index_buffer_data.push_back(i + 32 + 1);
	}

	tMesh = new Mesh("2DTerrain");
	tMesh->mode = Mesh::DRAW_TRIANGLE_STRIP ;

	glBindBuffer(GL_ARRAY_BUFFER, tMesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tMesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	tMesh->indexSize = index_buffer_data.size();
}

