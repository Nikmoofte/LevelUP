#include "Renderer.hpp"


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

