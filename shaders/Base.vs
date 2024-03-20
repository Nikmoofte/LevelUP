#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 total;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main()
{
    gl_Position = total * vec4(aPos, 1.0f);
}