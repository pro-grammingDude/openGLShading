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
     //ambient light
    float Ka = 0.1; //ambient coefficient
    vec3 ambient = Ka * defaultLightColor;
      
    vec3 lightDir = normalize(defaultLightPos - FragPos);
    
    float Ks = 1.0f; //specualr coefficient

    // Calculate the phong shading
    vec3 viewDir = normalize(-FragPos); // View direction
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), n);
    vec3 specular = Ks* spec * defaultLightColor;

    
    vec3 tempColor = (ambient+specular)*ourColor;
    FragColor = vec4(tempColor,1.0f);
}