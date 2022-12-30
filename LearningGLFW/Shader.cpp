#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    // ensure ifstream objects can throw exceptions:
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // open files
    vertexShaderFile.open(vertexShaderPath);
    fragmentShaderFile.open(fragmentShaderPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
    // close file handlers
    vertexShaderFile.close();
    fragmentShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    // shader Program
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertex);
    glAttachShader(shaderProgramID, fragment);
    glLinkProgram(shaderProgramID);
    // print linking errors if any
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() noexcept
{
    glDeleteProgram(shaderProgramID);
}

void Shader::Use()
{
    glUseProgram(shaderProgramID);
}

unsigned int Shader::GetID() const
{
    return shaderProgramID;
}

void Shader::SetBoolUniform(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}
void Shader::SetIntUniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void Shader::SetFloatUniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetVec4fUniform(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z, w);
}
