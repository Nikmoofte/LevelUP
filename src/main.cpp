#include <iostream>
#include <memory>
#include <chrono>

#include "Viewer/ShaderProg/ShaderProg.hpp"
#include <Renderer/Renderer.hpp>
#include <Camera/Camera.hpp>
#include <Viewer/Window/Window.hpp>
#include <Mesh/Mesh.hpp>
#include "path.hpp"
#include "config.hpp"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const glm::vec3 modelColor{0.5f};


float fov = glm::radians(90.0f);
std::chrono::time_point<std::chrono::system_clock> beg = std::chrono::system_clock::now();

int main()
{
    using namespace std;
    Viewer::Window window(SCR_WIDTH, SCR_HEIGHT);
    window.setTitle("CGA");
    Engine::Camera cam(glm::vec3{-5.0f, 5.0f, -5.0f}, glm::vec3{0.0f}, fov, glm::ivec2{SCR_WIDTH, SCR_HEIGHT});
    window.addOnCursorPositionChanged([&cam](double xpos, double ypos){cam.OnCursorPositionChanged(xpos, ypos);});
    window.addOnKeyChanged([&cam, &window](int key, int scancode, int action, int mods)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            window.setWindowShouldClose(true);
        cam.OnKeyChanged(key, scancode, action, mods);
    }
    );
    window.addOnMouseButtonChanged([&cam](int button, int action, int mods){cam.OnMouseButtonChanged(button, action, mods);});
    window.addOnResize([&cam](int width, int height)
    {
        glViewport(0, 0, width, height);
        cam.resize(fov, {width, height});
    }
    );

    Viewer::ShaderProg base(Path::Get().fromRoot({"shaders", "Base.vs"}), Path::Get().fromRoot({"shaders", "Base.fs"}));
    base.Use();

    //Изменяя строчку с .obj можно выьрать другой объект для отрисовки
    //ToDo попробовать другие объекты
    //https://sketchfab.com/feed -- здесь можно найти другие модельки. Нужны именно .obj
    //Можно просто выбрать из тех которые в папке 
    Assets::Mesh mesh(
        Path::Get().fromRoot({"assets", "models", "Doom Slayer", "doomslayer.obj"}).string(),
        base
    );
    Assets::Mesh chariot(
        Path::Get().fromRoot({"assets", "models", "Chariot.obj"}).string(), 
        base
    );

    mesh.Wait();
    chariot.Wait();

    //Тут можно ворочать свет, если надо
    glUniform3fv(base.GetLocation("lightPos"), 1, glm::value_ptr(glm::vec3(-5.0f, 5.0f, -5.0f)));  
    glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);
    mesh.setModelMat(glm::rotate(mesh.getModelMat(), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    mesh.setModelMat(glm::scale(mesh.getModelMat(), glm::vec3(5.0f)));
    while (!window.shouldClose())
    {
        cam.OnBeforeRender();
        glm::mat4 proj = cam.getProjectionMatrix();
        glm::mat4 view = cam.getViewMatrix();
        glUniform3fv(base.GetLocation("viewPos"), 1, glm::value_ptr(cam.getPos()));
        std::chrono::duration<float> time = std::chrono::system_clock::now() - beg;
        glUniform1f(base.GetLocation("time"), time.count());

        glm::mat4 model = mesh.getModelMat();
        //#todo Завтавить объект крутиться изменяя model


        //дальше трогать не требуется
        mesh.setModelMat(model);
        glm::mat4 total = proj * view; 
        glUniformMatrix4fv(base.GetLocation("total"), 1, GL_FALSE, glm::value_ptr(total));
        Renderer::Clear();
        glUniformMatrix4fv(base.GetLocation("model"), 1, GL_FALSE, glm::value_ptr(mesh.getModelMat()));
        Renderer::Draw(mesh);
        glUniformMatrix4fv(base.GetLocation("model"), 1, GL_FALSE, glm::value_ptr(chariot.getModelMat()));
        Renderer::Draw(chariot);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}


