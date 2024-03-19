#include "Shader.hpp"

#include <stdexcept>
#include <unordered_map>

GLuint Shader::create(const std::string& source)
{
    if(type == ShaderType::LAST_SHADER_TYPE)
        throw std::runtime_error("shader type has not been specified");
    

    id = glCreateShader(type.toGLdefine());
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    
    if(!isCompiled())
    {
        auto log = getCompileLog();
        throw std::runtime_error(
            type.toString() + 
            " id " + 
            std::to_string(id) + 
            " error log: \n" + 
            log
        );
    }
    
    return id;
}

GLuint Shader::createNoException(const std::string &source)
{
    if(type != ShaderType::LAST_SHADER_TYPE)
    {
        id = glCreateShader(type.toGLdefine());
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, NULL);
        glCompileShader(id);
        
        if(!isCompiled())
        {
            glDeleteShader(id);
            id = -1;
        }
    }
    return id;
}
inline Shader::ShaderType Shader::getType()
{
    return type;
}

inline void Shader::setType(ShaderType type)
{
    this->type = type;
}

inline bool Shader::isCompiled() 
{
    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    return result;
}

std::string Shader::getCompileLog()
{
    GLint result;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &result);
    char* buff = new char[result];
    glGetShaderInfoLog(id, result, &result, buff);
    std::string log;
    log += buff;
    return log;
}

std::unordered_map<GLuint, Shader::ShaderType::type_> SHTFromDMap{
        {GL_VERTEX_SHADER, Shader::ShaderType::VERTEX_SHADER}, 
        {GL_FRAGMENT_SHADER, Shader::ShaderType::FRAGMENT_SHADER}
    };
std::unordered_map<Shader::ShaderType::type_, GLuint> SHTToDMap{
        {Shader::ShaderType::VERTEX_SHADER, GL_VERTEX_SHADER}, 
        {Shader::ShaderType::FRAGMENT_SHADER, GL_FRAGMENT_SHADER}
    };
std::unordered_map<Shader::ShaderType::type_, std::string> SHTToSMap{
        {Shader::ShaderType::VERTEX_SHADER, "vertex shader"}, 
        {Shader::ShaderType::FRAGMENT_SHADER, "fragment shader"}
    };

Shader::ShaderType::ShaderType(GLuint t)
{
    type = SHTFromDMap[t];
}

Shader::ShaderType &Shader::ShaderType::operator=(const ShaderType &other)
{
    type = other.type;
    return *this;
}

Shader::ShaderType &Shader::ShaderType::operator=(ShaderType &&other)
{
    type = std::move(other.type);
    return *this;
}

Shader::ShaderType &Shader::ShaderType::operator=(type_ other)
{
    type = other;
    return *this;
}

Shader::ShaderType &Shader::ShaderType::operator=(GLuint other)
{
    type = SHTFromDMap[other];
    return *this;
}

GLuint Shader::ShaderType::toGLdefine()
{
    return SHTToDMap[type];
}

std::string Shader::ShaderType::toString()
{
    return SHTToSMap[type];
}
