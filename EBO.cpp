// template based on material from learnopengl.com
#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "importObj.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);



string GetFile(const char * filename)   //reads shader files
{
	ifstream file(filename, ios::in);
	string content;
	if(file.good())
	{
		stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
	}
	return content;
}


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



glm::mat4 transform = glm::mat4(1.0f); // transformation matrix

int main()
{
    float scaleX = 2.0f;
    float scaleY = 2.0f;
    float scaleZ= 2.0f;
    
    cout<<"Enter Model Number to render: 1. Dragon 2. Rose + Vase 3. Pawn 4. Head" <<endl;
    cout<<"Your Choice (1,2,3,4):";
    string modelNum,filepath;
    cin>>modelNum;
    if(modelNum.compare("1") == 0)
    {
        filepath = "Mesh/dragon.obj";
    }
    else if(modelNum.compare("2") == 0)
    {
        filepath = "Mesh/rose+vase.obj";
    }
    else if(modelNum.compare("3") == 0)
    {
        filepath = "Mesh/Pawn.obj";
    }
    else
    {
        filepath = "Mesh/head.obj";
    }

    cout<<"Enter Type of shading: 1.Gouraud 2.Phong 3.Flat 4.Z-Buffer Visualization"<<endl;
    cout<<"Your choice(1,2,3,4):";
    int shadingType;
    while (!(std::cin >> shadingType)) {
        std::cout << "Invalid input. Please enter a valid integer choice (1, 2, 3, 4): ";

        // Clear the input buffer to handle non-integer input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    string content_of_ShaderFile;
    string content_of_file;
    if(shadingType == 1)
    {
        content_of_ShaderFile = GetFile("VertexShaderSource.vert"); //reads the content of VertexShader file
        content_of_file = GetFile("FragmentShaderSource.fs"); //reads the content of fragment shader from the file
    }
    else if(shadingType == 3)
    {
        content_of_ShaderFile = GetFile("PhongVertexShader.vert");
        content_of_file = GetFile("FlatShading.fs");
    }
    else if(shadingType == 4)
    {
        content_of_ShaderFile = GetFile("PhongVertexShader.vert");
        content_of_file = GetFile("Z_Buffer.fs");
    }
    else
    {
        content_of_ShaderFile = GetFile("PhongVertexShader.vert"); //reads the content of VertexShader file
        content_of_file = GetFile("phong.fs"); //reads the content of fragment shader from the file
    }
   
    
    char *vertexShaderSource = &content_of_ShaderFile[0];
    
    char *fragmentShaderSource = &content_of_file[0];
    
   

    vector<Vertex> vertices;
    vector<Normal> normals;
    vector<Face> faces;
    
    const char *filepathPointer = &filepath[0];
    readObjFile(filepathPointer,vertices,normals,faces);
    
    GLuint indices[faces.size()];
   // cout<<"VERTICES SIZE: "<<vertices.size()<< " NORMALS SIZE:" << normals.size()<<endl;
    for(int i=0;i<faces.size();i+=1) //assigning normals to their respective vertex and creating array of vertexIndex to send to EBO
    {
        indices[i] = faces[i].vertexIndex;
        //cout<<faces[i].vertexIndex<<" "<<faces[i].normalIndex<<endl;
        vertices[faces[i].vertexIndex].normal = normals[faces[i].normalIndex];
    }
    
    vector<float> vertexData;
    for(int i=0;i<vertices.size();i+=1)     // (VVV CCC NNN) sorting data in this order
    {
        vertexData.push_back(vertices[i].x);
        vertexData.push_back(vertices[i].y);
        vertexData.push_back(vertices[i].z);
        glm::vec3 color(0.0f);
        if(i%3 == 0)
        {
            color = glm::vec3(0.0f,1.0f,0.0f);
        }
        else if(i%3 == 1)
        {
            color = glm::vec3(0.0f,1.0f,0.0f);
        }
        else
        {
            color = glm::vec3(0.0f,1.0f,0.0f);
        }
        vertexData.push_back(color.x);
        vertexData.push_back(color.y);
        vertexData.push_back(color.z);
        vertexData.push_back(vertices[i].normal.x);
        vertexData.push_back(vertices[i].normal.y);
        vertexData.push_back(vertices[i].normal.z);
        
    }
    
    // for(int i=0;i<vertexData.size();i+=1)
    // {
    //     cout<<vertexData[i]<<" ";
    // }
    
    
    // Set up a perspective projection
    glm::mat4 projection = glm::perspective(glm::radians(30.f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 1.0f, 150.0f);   
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,6),glm::vec3(0,0,0),glm::vec3(0,1,0));   
    
    
    
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    cout<<glGetError();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

   
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    // GLuint vbo_normals;
    // glGenBuffers(1, &vbo_normals); //working with 2 VBOs giving errors figure it out
    

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float) , vertexData.data() , GL_STATIC_DRAW);    

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices , GL_STATIC_DRAW);
  

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    
    

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    
        
               
   glEnable(GL_DEPTH_TEST);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // input
        // -----
        processInput(window);

        
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //assign view matrix
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        //assign projection matrix
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // get matrix's uniform location and set matrix 
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, sizeof(indices)/3, GL_UNSIGNED_INT, 0); 
        
        

        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)    //right
        transform = glm::translate(transform, glm::vec3(0.001f, 0.0f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)    //left
        transform = glm::translate(transform, glm::vec3(-0.001f, 0.0f, 0.0f));
     if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)   //up
        transform = glm::translate(transform, glm::vec3(0.0f, 0.001f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)    //down
        transform = glm::translate(transform, glm::vec3(0.0f, -0.001f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //rotate left
        transform = glm::rotate(transform, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)    //rotate right
        transform = glm::rotate(transform, glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)   //rotate up
        transform = glm::rotate(transform, glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //rotate down
        transform = glm::rotate(transform, glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
     if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //scale
        transform = glm::scale(transform,glm::vec3( 1.0002f, 1.0002f ,1.0002f));
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) //scale down
        transform = glm::scale(transform,glm::vec3( 0.9999f, 0.9999f ,0.9999f));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}