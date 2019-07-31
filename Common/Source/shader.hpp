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

	void SetUniform(std::string uniformName, bool b);
	void SetUniform(std::string uniformName, int i);
	void SetUniform(std::string uniformName, float f);

	void SetUniform(std::string uniformName, vec3 v);
	void SetUniform(std::string uniformName, mat4 m);
};

#endif
