#ifndef SHADER_HPP
#define SHADER_HPP

#include "Vector3.h"
#include "Mtx44.h"

#include <string>

class Shader
{
private:
	unsigned int ID;
	void CheckStatus(short type, unsigned int shader); // check if compiled or not
	// codes:
	// 0: program
	// 1: vertex
	// 2: fragment
public:
	void Use();

	Shader();
	Shader(std::string vertexFilePath, std::string fragmentFilePath);
	~Shader();

	void Init(std::string vertexFilePath, std::string fragmentFilePath);

	void SetBool(std::string uniformName, bool b);
	void SetInt(std::string uniformName, int i);
	void SetFloat(std::string uniformName, float f);

	void SetVec3(std::string uniformName, Vector3 v);
	void SetMat4(std::string uniformName, Mtx44 m);
};

#endif
