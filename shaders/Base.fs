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
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * vec3(Color);
    FragColor = vec4(result, Color.w);
}