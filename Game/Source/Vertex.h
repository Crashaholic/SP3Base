#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"

struct Position
{
	float x, y, z;
	Position(float x = 0, float y = 0, float z = 0) {Set(x, y, z);}
	void Set(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
	operator Vector3() { return Vector3(x, y, z); };
};

struct Color
{
	float r, g, b;
	Color(float r = 1, float g = 1, float b = 1) {Set(r, g, b);}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}
	void Set(std::string color) {
		std::string temp;
		int mode = 0;
		for (int i = 0; i < color.length(); ++i)
		{
			if (color[i] == ',')
			{
				switch (mode) {
				case 0:
					r = stof(temp);
					break;
				case 1:
					g = stof(temp);
					break;
				case 2:
					b = stof(temp);
					break;
				}
				++i;
				temp = "";
				++mode;
			} 
			temp += color[i];
		} switch (mode) {
		case 0:
			r = stof(temp);
			break;
		case 1:
			g = stof(temp);
			break;
		case 2:
			b = stof(temp);
			break;
		}
	}
	operator Vector3() { return Vector3(r, g, b); };
};

struct TexCoord
{
	float u, v;
	TexCoord(float u = 0, float v = 0) {Set(u, v);}
	void Set(float u, float v) {this->u = u; this->v = v;}
	operator Vector3() { return Vector3(u, v, 0); };
};

struct Vertex 
{
	Position pos;
	Color color;
	Vector3 normal;
	TexCoord texCoord;
	Vertex(){}
};

#endif