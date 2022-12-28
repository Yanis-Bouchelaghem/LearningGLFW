#pragma once
#include <string>
class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void Use();
	// utility uniform functions
	void setBoolUniform(const std::string& name, bool value) const;
	void setIntUniform(const std::string& name, int value) const;
	void setFloatUniform(const std::string& name, float value) const;
private:
	unsigned int shaderProgramID;
};