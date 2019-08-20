#pragma once
#include "Vector3.h"
#include "MeshBuilder.h"
#include <vector>

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
	Vector3 GetHeight(Vector3 TankPos);

	void GenerateRandomHeight(float worldWidth);
	void GenerateTerrainMesh();
	void DeformTerrain(Vector3 ExplosionPosition, float ExplosionRadius);
};

