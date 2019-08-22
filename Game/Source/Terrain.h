#pragma once
#include "Vector3.h"
#include "MeshBuilder.h"
#include <vector>

#define TERRAIN_SIZE 16
#define TERRAIN_UP 5
#define TERRAIN_DN -5

class Terrain
{
public:

	Terrain();
	~Terrain();

	Vector3 Points[TERRAIN_SIZE];
	Vector3 NewPoints[TERRAIN_SIZE];
	Vector3 Bottom[TERRAIN_SIZE];

	Mesh* tMesh;

	Vector3 GetNormal(Vector3 ObjectPosition);
	Vector3 GetHeight(Vector3 TankPos);

	void Update(double dt);
	void GenerateRandomHeight(float worldWidth);
	void GenerateTerrainMesh();
	void DeformTerrain(Vector3 ExplosionPosition, float ExplosionRadius);
};

