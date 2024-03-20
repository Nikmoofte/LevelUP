#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 total;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;



void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNorm;  
    
    gl_Position = total * vec4(FragPos, 1.0);
}