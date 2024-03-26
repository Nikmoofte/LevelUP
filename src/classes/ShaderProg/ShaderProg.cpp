#include "ShaderProg.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "Shader/Shader.hpp"

using namespace std::filesystem;

ShaderProg::ShaderProg(const path &vertexFilepath, const path &fragmentFilepath)
{
    id = CreateProgram(GetSourceFromFile(vertexFilepath), GetSourceFromFile(fragmentFilepath));
}

ShaderProg::ShaderProg(const ShaderProg &other)
{
    this->id = other.id;
}

ShaderProg::~ShaderProg()
{
    glDeleteProgram(this->id);
}

void ShaderProg::Use() const
{
    glUseProgram(this->id);
}

void ShaderProg::UnUse() const
{
    glUseProgram(0);
}

GLint ShaderProg::GetLocation(const std::string &name)
{ 
    if(UniformLocCache.find(name) != UniformLocCache.end())
        return UniformLocCache[name];

    GLint location = glGetUniformLocation(id, name.c_str());
    if(location == -1)
        std::cerr << "Uniform " + name + " not found!" << std::endl;
    else
        UniformLocCache[name] = location;
    return location;
}


unsigned ShaderProg::CreateProgram(const std::string &vertexSource, const std::string &fragmentSource)
{
    unsigned int shaderProgram = glCreateProgram();
    
    Shader vertex(GL_VERTEX_SHADER, vertexSource);
    Shader fragment(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(shaderProgram, vertex.getID());
    glAttachShader(shaderProgram, fragment.getID());
    glLinkProgram(shaderProgram);

    int result;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &result);
        char* log = new char[result];
        glGetProgramInfoLog(shaderProgram, result, &result, log);
        std::string message("Program failed to link: \n");
        message += log;
        message += vertexSource + "\n\n";    
        message += fragmentSource + "\n\n";
        throw std::runtime_error(message);
    }
    
    return shaderProgram;
}

std::string ShaderProg::GetSourceFromFile(const path &filePath)
{
    std::ifstream in(filePath);
    std::stringstream sIn;
    sIn << in.rdbuf();
    return sIn.str();
}
