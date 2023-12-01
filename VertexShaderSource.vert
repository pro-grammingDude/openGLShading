#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor;


const vec3 defaultLightPos = vec3(-5.0, 10.0, -3.0); // Default light position
const vec3 defaultLightColor = vec3(1, 1, 1); // White light color
float n = 10;

void main()
{
    //ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * defaultLightColor;

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec3 lightDir = normalize(defaultLightPos - FragPos);
    
    // Calculate the diffuse reflection (Lambertian reflection)
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * defaultLightColor;    

    
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = (diffuse+ambient)*aColor;
}
