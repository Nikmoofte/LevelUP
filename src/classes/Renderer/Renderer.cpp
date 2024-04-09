#include "Renderer.hpp"

#include "Mesh/Mesh.hpp"
#include "Viewer/ShaderProg/ShaderProg.hpp"

using Viewer::ShaderProg;
void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VAO &va, const EBO &ib, const ShaderProg &prog)
{
    va.Bind();
    ib.Bind();
    prog.Use();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
}
void Renderer::Draw(const Assets::Mesh& mesh)
{
    Draw(mesh.vao, mesh.ebo, mesh.GetShaderProg());
}

