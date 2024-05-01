#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "tp02/config.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Taille de la fenêtre
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// Fonction pour gérer les entrées utilisateur
void processInput(GLFWwindow *window)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
      }
}


int main() 
{

// ... (Déclarations de variables)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Code du shader vertex
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
const char* vertexShaderSource = vShaderCode;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Code du shader fragment
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
const char* fragmentShaderSource=fShaderCode;
//layout (location = 0) in vec3 aPos;
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Initialisation de GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


// Création de la fenêtre GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "TP 2 DE CUBE", NULL, NULL);
    if (window == NULL)
        {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
        }
    glfwMakeContextCurrent(window);             // sans cette ligne pas de glad

// Initialisation de GLAD
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

// Configuration du test de profondeur
glEnable(GL_DEPTH_TEST); 
//glDisable(GL_DEPTH_TEST); 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des données des vertices 
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

    // Création et configuration des buffers
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // ... (Code pour lier et configurer les buffers)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);  

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Boucle principale de rendu  
    while(!glfwWindowShouldClose(window)){
    // Gestion des entrées utilisateur
    processInput(window);  

    // Effacement du tampon de couleur et du tampon de profondeur
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Utilisation du programme de shaders
    glUseProgram(shaderProgram);

    // ... (Code pour définir les matrices de transformation)
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


    // Liaison du tableau de vertices
    glBindVertexArray(VAO);

    // Rendu des vertices
    glDrawArrays(GL_TRIANGLES, 0, 36);         //on met le nombre de point (36)
    // glBindVertexArray(0); // no need to unbind it every time

    // Échange des tampons et gestion des événements
    glfwSwapBuffers(window);
    glfwPollEvents();    
    }

    // ... (Nettoyage des ressources)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    std::cout << "Hello world !" << std::endl;
    return 0;
}

