#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader
{

public:
    struct ShaderType
    {
        enum type_
        {
            VERTEX_SHADER,
            FRAGMENT_SHADER,
            LAST_SHADER_TYPE
        } type;
        
        ShaderType(type_ t) : type(t) {}
        ShaderType(GLuint t);
        ShaderType(const ShaderType& t) : type(t.type) {}
        ShaderType(ShaderType&& t) : type(std::move(t.type)) {}
        ShaderType& operator=(const ShaderType& other); 
        ShaderType& operator=(ShaderType&& other); 
        ShaderType& operator=(type_ other); 
        ShaderType& operator=(GLuint other); 

        operator type_() { return type; }
        GLuint toGLdefine();
        std::string toString();
    };

    Shader() = default;
    Shader(ShaderType type) : type(type) {}
    Shader(ShaderType type, const std::string& source) : type(type) { create(source); } 
    ~Shader() { glDeleteShader(id); } 

    GLuint create(const std::string& source);
    GLuint createNoException(const std::string& source);

    inline GLuint getID() { return id; } ;
    inline ShaderType getType();
    inline void setType(ShaderType type);

private:
    inline bool isCompiled();
    std::string getCompileLog();
    ShaderType type = ShaderType::LAST_SHADER_TYPE;
    GLuint id = -1;
};

#endif