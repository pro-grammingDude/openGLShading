#version 330 core
out vec4 FragColor;

float near_Plane = 0.1; 
float far_Plane  = 100.0; 
  
float convertToZ(float depth) 
{
    float z = depth * 2.0 - 1.0;  
    return (2.0 * near_Plane * far_Plane) / (far_Plane + near_Plane - z * (far_Plane - near_Plane));	
}

void main()
{             
    float depth = convertToZ(gl_FragCoord.z);    
    //FragColor = vec4(vec3(gl_FragCoord.z),1.0);
    FragColor = vec4(vec3(depth), 1.0);
    
}