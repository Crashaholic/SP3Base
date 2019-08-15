#pragma once
#include "Vector3.h"
#include "MeshBuilder.h"

#define TERRAIN_SIZE 16
#define TERRAIN_UP 3
#define TERRAIN_DN -3

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
	Vector3 getHeight(Vector3 TankPos);

};

