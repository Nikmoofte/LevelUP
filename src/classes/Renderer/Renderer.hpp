#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <cstddef>
#include <glm/vec3.hpp>

#include "VAO/VAO.hpp"
#include "EBO/EBO.hpp"
//#include "ShaderProg/ShaderProg.hpp"


namespace Assets
{
    class Mesh;
}
namespace Viewer
{
    class ShaderProg;
}

class Renderer
{
public:
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    ~Renderer() = delete;
    static void Clear();
    static void Draw(const VAO&, const EBO&, const Viewer::ShaderProg&);
    static void Draw(const Assets::Mesh& mesh);
private:


};

#endif