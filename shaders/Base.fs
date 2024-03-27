#version 330

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;  
in vec3 FragPos;  
in vec4 Color;  


void main()
{
    vec3 lightColor = vec3(1.0f);

    //ToDo Сделать свет по фонгу
    //https://learnopengl.com/Lighting/Basic-Lighting
    
    vec3 result =  vec3(Color);
    FragColor = vec4(result, Color.w);
}