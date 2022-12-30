#pragma once
#include <string>
class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	~Shader() noexcept;

	void Use();
	unsigned int GetID() const;
	// utility uniform functions
	void SetBoolUniform(const std::string& name, bool value) const;
	void SetIntUniform(const std::string& name, int value) const;
	void SetFloatUniform(const std::string& name, float value) const;
	void SetVec4fUniform(const std::string& name, float x, float y, float z, float w) const;
private:
	unsigned int shaderProgramID;
};