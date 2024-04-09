#ifndef SHADERPROG_H
#define SHADERPROG_H

#include <glad/glad.h>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace Viewer
{
    class ShaderProg
    {
    public:
        ShaderProg(const std::filesystem::path& vertexFilepath, const std::filesystem::path& fragmentFilepath);
        ShaderProg(ShaderProg&&);
        ShaderProg(const ShaderProg&);
        ~ShaderProg();

        void Use() const;
        void UnUse() const;

        GLint GetLocation(const std::string& name);
    private:
        GLuint CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);
        std::string GetSourceFromFile(const std::filesystem::path& filePath);
        std::unordered_map<std::string, int> UniformLocCache;
        GLuint id;
    };
}

#endif