cmake_minimum_required(VERSION 3.5)
project(WinOPGLProj)

set(ClassesDir src/classes/)
set(ImguiPath deps/imgui/)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512bw -mavx512cd -mavx512dq")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Classes 
    ${ClassesDir}Camera/Camera.cpp
    ${ClassesDir}Renderer/Renderer.cpp
    ${ClassesDir}VBO/VBO.cpp
    ${ClassesDir}VAO/VAO.cpp
    ${ClassesDir}VAO/VBlayout.cpp
    ${ClassesDir}EBO/EBO.cpp
    ${ClassesDir}Mesh/Mesh.cpp

    ${ClassesDir}Viewer/ShaderProg/ShaderProg.cpp
    ${ClassesDir}Viewer/Shader/Shader.cpp
    ${ClassesDir}Viewer/Window/Window.cpp
    ${ClassesDir}Viewer/Menu/Menu.cpp
    #${ClassesDir}Viewer/App/App.cpp

    ${ImguiPath}imgui_demo.cpp
    ${ImguiPath}imgui_draw.cpp
    ${ImguiPath}imgui_tables.cpp
    ${ImguiPath}imgui_widgets.cpp
    ${ImguiPath}imgui.cpp
    ${ImguiPath}backends/imgui_impl_opengl3.cpp
    ${ImguiPath}backends/imgui_impl_glfw.cpp
)

add_executable(main src/main.cpp include/glad/glad.c include/stb_image/stb_image.cpp ${Classes})

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/tinyobjloader)
target_link_libraries(main PUBLIC glfw
                                tinyobjloader
                                glm
                                opengl32)
target_include_directories(main PUBLIC include)
target_include_directories(main PUBLIC ${ClassesDir})
target_include_directories(main PUBLIC ${ImguiPath})