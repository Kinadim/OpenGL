#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "tp02_ILU/config.hpp"


//ta pas encore fais le tp
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "TP 02 ILLUMINATION", NULL, NULL);
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




// don't forget to use the corresponding shader program first (to set the uniform)
//////////////////////////////////////////////////shaderProgram.use();
//////////////////////////////////////////////////shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
//////////////////////////////////////////////////shaderProgram.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

//glDisable(GL_DEPTH_TEST); 
// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
std::vector<float> vertices = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0  0
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 1  1
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f, 2.0f, // 2  2
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f, 3.0f, // 3  3

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 4.0f, // 0  4
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 5.0f, // 1  5
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   2.0f, 6.0f, // 2  6
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   3.0f, 7.0f, // 3  7

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 8.0f, // 0  8
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 9.0f, // 1  9
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  2.0f, 10.0f, // 2 10
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  3.0f, 11.0f, // 3 11

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 12.0f, // 0 12
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 13.0f, // 1 13
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  2.0f, 14.0f, // 2 14
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  3.0f, 15.0f, // 3 15

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 16.0f, // 0 16
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 17.0f, // 1 17
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  2.0f, 18.0f, // 2 18
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  3.0f, 19.0f, // 3 19

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 20.0f, // 0 20
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 21.0f, // 1 21
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  2.0f, 22.0f, // 2 22
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  3.0f, 23.0f  // 3 23
    };

    std::vector<unsigned int> indices = {
        2,  1,  0, // 0 1 2
        0,  3,  2, // 2 3 0

        4,  5, 6,
        6,  7, 4,

        8,  9, 10,
        10, 11,  8,

        14, 13, 12,  // 12 13 14
        12, 15,  14, //14 15 12

        16, 17, 18,
        18, 19, 16,

        22, 21, 20, // 20 21 22
        20, 23, 22  // 22 23 20
    };


unsigned int VBO;
unsigned int VAO;
glGenBuffers(1, &VBO);
glGenVertexArrays(1, &VAO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);  

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);

glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
glEnableVertexAttribArray(2);

//glBindBuffer(GL_ARRAY_BUFFER, VBO);

// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

/*
unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW); 

     // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
*/
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW); 
    //Pour la perspective//
    glEnable(GL_DEPTH_TEST); 

    // set the vertex attribute 
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);  

        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw our first triangle
        glUseProgram(shaderProgram);
        // create transformations
        glm::mat4 view       = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 model      = glm::mat4(1.0f);

        ////////projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));



        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
        //glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);

        int lightLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3f(lightLoc,1.0f, 1.0f, 1.0f);






        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);         //on met le nombre de point (36)
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

