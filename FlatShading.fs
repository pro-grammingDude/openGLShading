#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;

const vec3 defaultLightPos = vec3(-5.0, 10.0, -3.0); // Default light position
const vec3 defaultLightColor = vec3(1, 1, 1); // White light color
const float n = 10.0f;


void main()
{
    vec3 lightDirection = normalize(defaultLightPos - FragPos);
    vec3 x_derivative = dFdx(FragPos);
    vec3 y_derivative = dFdy(FragPos);
    vec3 faceNormal = normalize(cross(x_derivative, y_derivative));
    float flatShading = dot(lightDirection, faceNormal);
    FragColor = vec4(flatShading * ourColor,1.0);
    
}