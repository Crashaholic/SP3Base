#ifndef BOMB_H
#define BOMB_H
#include "../Terrain.h"

//#include "GameObject.h"
#include "Vector3.h"

class Bomb
{
private:
	Terrain* terreference;
protected:

public:
	void SetTerrain(Terrain* terr);
	Bomb();
	~Bomb();

};

#endif