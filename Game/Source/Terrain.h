#pragma once
#include "Vector3.h"
#include "MeshBuilder.h"

#define TERRAIN_SIZE 64

class Terrain
{
public:

	Terrain();
	~Terrain();

	Vector3 Points[TERRAIN_SIZE];
	Vector3 Bottom[TERRAIN_SIZE];

	Mesh* tMesh;

	Vector3 GetNormal(Vector3 ObjectPosition);

	void GenerateRandomHeight(unsigned int worldWidth);
	void GenerateTerrainMesh();

};

