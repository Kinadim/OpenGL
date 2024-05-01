#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "tp03/config.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
void processInput(GLFWwindow *window)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
      }
}


int main() 
{
        // Code du shader
std::string vertex_shader_code;

// Ouverture du fichier
std::ifstream vertex_shader_file;
vertex_shader_file.open(std::string(_resources_directory).append("shaders/lighting.vert").c_str());

// Lecure du fichier
std::stringstream iss;
iss << vertex_shader_file.rdbuf();
vertex_shader_file.close();

// Conversion du stringstream en string
vertex_shader_code = iss.str();

// Conversion en chaine C
const char* vShaderCode = vertex_shader_code.c_str();
    const char *vertexShaderSource = vShaderCode;


    // Code du shader
std::string fragment_shader_code;

// Ouverture du fichier
std::ifstream fragment_shader_file;
fragment_shader_file.open(std::string(_resources_directory).append("shaders/lighting.frag").c_str());

// Lecure du fichier
std::stringstream ess;
ess << fragment_shader_file.rdbuf();
fragment_shader_file.close();

// Conversion du stringstream en string
fragment_shader_code = ess.str();

// Conversion en chaine C
const char* fShaderCode = fragment_shader_code.c_str();
    const char *fragmentShaderSource=fShaderCode;
//layout (location = 0) in vec3 aPos;
    // ça part de là !
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
        {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
        }
    glfwMakeContextCurrent(window);             // sans cette ligne pas de glad
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
        // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

// check for shader compile errors
int  success;
char infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}

 // fragment shader
 unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);

 // check for shader compile errors
glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
}

// link shaders
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
// check for linking errors
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
}
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
/*std::vector<float> vertices = {
  // position            // colors
  -0.5f, -0.5f, -0.5f,   
   0.5f, -0.5f, -0.5f,   
   0.5f,  0.5f, -0.5f,   
   0.5f,  0.5f, -0.5f,   
  -0.5f,  0.5f, -0.5f,   
  -0.5f, -0.5f, -0.5f,   

  -0.5f, -0.5f,  0.5f,   
   0.5f, -0.5f,  0.5f,   
   0.5f,  0.5f,  0.5f,   
   0.5f,  0.5f,  0.5f,   
  -0.5f,  0.5f,  0.5f,   
  -0.5f, -0.5f,  0.5f,   

  -0.5f,  0.5f,  0.5f,   
  -0.5f,  0.5f, -0.5f,   
  -0.5f, -0.5f, -0.5f,   
  -0.5f, -0.5f, -0.5f,   
  -0.5f, -0.5f,  0.5f,   
  -0.5f,  0.5f,  0.5f,   

   0.5f,  0.5f,  0.5f,   
   0.5f,  0.5f, -0.5f,   
   0.5f, -0.5f, -0.5f,   
   0.5f, -0.5f, -0.5f,   
   0.5f, -0.5f,  0.5f,   
   0.5f,  0.5f,  0.5f,   

  -0.5f, -0.5f, -0.5f,   
   0.5f, -0.5f, -0.5f,   
   0.5f, -0.5f,  0.5f,   
   0.5f, -0.5f,  0.5f,   
  -0.5f, -0.5f,  0.5f,   
  -0.5f, -0.5f, -0.5f,   
  
  -0.5f,  0.5f, -0.5f,   
   0.5f,  0.5f, -0.5f,   
   0.5f,  0.5f,  0.5f,   
   0.5f,  0.5f,  0.5f,   
  -0.5f,  0.5f,  0.5f,   
  -0.5f,  0.5f, -0.5f,   
}; */

std::vector<float> vertices = {
  // position            // colors
  
  -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    
   
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

   0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f

};
unsigned int VBO;
unsigned int VAO;
glGenBuffers(1, &VBO);
glGenVertexArrays(1, &VAO);
// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);  

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
glBindBuffer(GL_ARRAY_BUFFER, 0);

// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
glBindVertexArray(0);

    while(!glfwWindowShouldClose(window))
    {
    processInput(window);  
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    
    glfwTerminate();

    std::cout << "Hello world !" << std::endl;
    return 0;
}

