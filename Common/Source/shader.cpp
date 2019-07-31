#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <GL/glew.h>
#include "shader.hpp"
#include "Logging.h"

void Shader::CheckStatus(short s, unsigned int ui)
{
	int state;
	char infoLog[1024];
	switch (s)
	{
	case 0:
	{
		glGetProgramiv(ui, GL_LINK_STATUS, &state);
		if (!state)
		{
			glGetProgramInfoLog(ui, 1024, NULL, infoLog);
			LOG_ERROR("[PROGRAM] LINKING FAILED: \n%\n", infoLog);
			//std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	case 1:
	{
		glGetShaderiv(ui, GL_COMPILE_STATUS, &state);
		if (!state)
		{
			glGetShaderInfoLog(ui, 1024, NULL, infoLog);
			LOG_ERROR("[SHADER] [VERT] COMPILE FAILED: \n%\n", infoLog);
			//std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	case 2:
	{
		glGetShaderiv(ui, GL_COMPILE_STATUS, &state);
		if (!state)
		{
			glGetShaderInfoLog(ui, 1024, NULL, infoLog);
			LOG_ERROR("[SHADER] [FRAG] COMPILE FAILED: \n%\n", infoLog);
			//std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	}
}

Shader::Shader()
{
}

Shader::Shader(std::string v, std::string f)
{
	std::string vCode, fCode;
	std::ifstream vFile, fFile;
	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	// === VERTEX ===
	try
	{
		std::stringstream vStream;
		vFile.open(v.c_str());
		vStream << vFile.rdbuf();
		vFile.close();
		vCode = vStream.str();
	}
	catch (std::ifstream::failure e)
	{
		LOG_ERROR("[SHADER] [VERTEX] CANNOT OPEN: %", v);
	}
	// === FRAGMENT ===
	try
	{
		std::stringstream fStream;
		fFile.open(f.c_str());
		fStream << fFile.rdbuf();
		fFile.close();
		fCode = fStream.str();
	}
	catch (std::ifstream::failure e)
	{
		LOG_ERROR("[SHADER] [FRAGMENT] CANNOT OPEN: %", f);
	}
	const char* vCodeCStr = vCode.c_str();
	const char* fCodeCStr = fCode.c_str();
	unsigned int vert, frag;
	
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vCodeCStr, NULL);
	glCompileShader(vert);
	CheckStatus(1, vert);

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fCodeCStr, NULL);
	glCompileShader(frag);
	CheckStatus(2, frag);

	ID = glCreateProgram();
	glAttachShader(ID, vert);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	CheckStatus(0, ID);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader()
{
	
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetUniform(std::string uniformName, bool b)
{
	glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), b);
}

void Shader::SetUniform(std::string uniformName, int i)
{
	glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), i);
}

void Shader::SetUniform(std::string uniformName, float f)
{
	glUniform1f(glGetUniformLocation(ID, uniformName.c_str()), f);
}

void Shader::SetUniform(std::string uniformName, vec3 v)
{
	glUniform3fv(glGetUniformLocation(ID, uniformName.c_str()), 1, &v.x);
}

void Shader::SetUniform(std::string uniformName, mat4 m)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, uniformName.c_str()), 1, GL_FALSE, &m.a[0]);
}
