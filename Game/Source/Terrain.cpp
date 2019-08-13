#include "Terrain.h"
#include "MyMath.h"
#include "Logging.h"

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::GenerateRandomHeight()
{
	for (int i = 0; i < 64; ++i)
	{
		Points[i].y = Math::RandFloatMinMax(2.f, 20.f);
		LOG_NONE("Points[%]: %", i, Points[i]);
	}
}

