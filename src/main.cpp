#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ShaderProg/ShaderProg.hpp"
#include "Shader/Shader.hpp"
#include <VBO/VBO.hpp>
#include <VAO/VAO.hpp>
#include <VAO/VBLayout.hpp>
#include <EBO/EBO.hpp>
#include <Renderer/Renderer.hpp>
#include <Camera/Camera.hpp>
#include <Mesh/Mesh.hpp>
#include <memory>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void cursorPosChanged(GLFWwindow* window, double xpos, double ypos);
void mouseButtonPressed(GLFWwindow* window, int button, int action, int mods);
void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::unique_ptr<Engine::Camera> cam;

int main()
{
    using namespace std;
    cout << sizeof(Shader) << " " << sizeof(Shader::ShaderType) << endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ShaderProg base("../shaders/Base.vs", "../shaders/Base.fs");
    base.Use();
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    Assets::Mesh mesh("../assets/models/Chariot.obj");

    VBO object;
    VAO va;
    mesh.Wait();
    VBLayout vbl;
    object.SetBufferData(mesh.getVerticesSize(), &mesh.getVertices().front(), GL_STATIC_DRAW);

    vbl.Push<float>(3);
    vbl.Push<float>(3);
    vbl.Push<float>(2);

    va.setLayout(object, vbl);
    EBO eb;
    eb.SetBufferData(mesh.getIndeciesSize(), &mesh.getIndecies().front(), GL_STATIC_DRAW);
    
    cam = std::make_unique<Engine::Camera>(glm::vec3{0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::radians(90.0f), glm::ivec2{SCR_WIDTH, SCR_HEIGHT});
    glfwSetCursorPosCallback(window, cursorPosChanged);
    glfwSetMouseButtonCallback(window, mouseButtonPressed);
    glfwSetKeyCallback(window, keyPressed);


    glm::mat4 proj = cam->getProjectionMatrix();
    //glUniformMatrix4fv(base.GetLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));
      
    glClearColor(0.4f, 0.6f, 0.8f, 1.0f);

    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {
        cam->OnBeforeRender();
        glm::mat4 view = cam->getViewMatrix();
        // glUniformMatrix4fv(base.GetLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(base.GetLocation("model"), 1, GL_FALSE, glm::value_ptr(mesh.getModelMat()));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glm::mat4 model = mesh.getModelMat();
        //#todo Завтавить объект крутиться изменяя model


        //дальше трогать не требуется
        mesh.setModelMat(model);
        glm::mat4 total = proj * view * mesh.getModelMat(); 
        glUniformMatrix4fv(base.GetLocation("total"), 1, GL_FALSE, glm::value_ptr(total));
        Renderer::Clear();
        Renderer::Draw(va, eb, base);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void cursorPosChanged(GLFWwindow *window, double xpos, double ypos)
{
    if(cam.get())
        cam->OnCursorPositionChanged(xpos, ypos);
}

void mouseButtonPressed(GLFWwindow *window, int button, int action, int mods)
{
    if(cam.get())
        cam->OnMouseButtonChanged(button, action, mods);
}

void keyPressed(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(cam.get())
        cam->OnKeyChanged(key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    auto c = cam.get(); 
    if(c != nullptr)
        c->resize(glm::radians(90.0f), {width, height});
}

