#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <glengine/orbitalCamera.hpp>
#include "tp01_ANI/config.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Variables globales
// État de la camera
bool firstMouse = true;
float lastX;
float lastY;

enum class MousePressedButton
{
    NONE,
    LEFT,
    RIGHT,
    MIDDLE
};
MousePressedButton mouseButtonState = MousePressedButton::NONE;
 // La camera
GLEngine::OrbitalCamera orbitalCamera(glm::vec3(0.3f, 0.4f, 3.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_RELEASE)
    {
        mouseButtonState = MousePressedButton::NONE;
    }
    else
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButtonState = MousePressedButton::LEFT;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseButtonState = MousePressedButton::RIGHT;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButtonState = MousePressedButton::MIDDLE;
            break;
        }
    }
}

void onMouseMove(GLFWwindow *window, double xpos, double ypos)
{
    if (mouseButtonState == MousePressedButton::NONE)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
    }
    else
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = (float)xpos - lastX;
        float yoffset = lastY - (float)ypos;

        lastX = (float)xpos;
        lastY = (float)ypos;

        switch (mouseButtonState)
        {
        case MousePressedButton::LEFT:
            orbitalCamera.orbit(xoffset, yoffset);
            break;
        case MousePressedButton::RIGHT:
            orbitalCamera.track(xoffset);
            orbitalCamera.pedestal(yoffset);
            break;
        case MousePressedButton::MIDDLE:
            orbitalCamera.dolly(yoffset);
            break;
        case MousePressedButton::NONE:
            break;
        }
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
    const char *vShaderCode = vertex_shader_code.c_str();
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
    const char *fShaderCode = fragment_shader_code.c_str();
    const char *fragmentShaderSource = fShaderCode;
    // layout (location = 0) in vec3 aPos;



    //  ça part de là !
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "TP 01 ILLUMINATION", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

   
    // Gestionnaire d'événements pour GLFW
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onMouseMove);
    // glfwSetScrollCallback(window, onMouseScroll);

    glfwMakeContextCurrent(window); // sans cette ligne pas de glad
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // don't forget to use the corresponding shader program first (to set the uniform)
    //////////////////////////////////////////////////shaderProgram.use();
    //////////////////////////////////////////////////shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    //////////////////////////////////////////////////shaderProgram.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
    // Pour la perspective//
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    //  set up vertex data (and buffer(s)) and configure vertex attributes
    //  ------------------------------------------------------------------
    std::vector<float> vertices = {
  // position            // colors
  
  -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
    
   
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,

  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,

   0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
   0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,

  -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
   0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,

  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,

};
    unsigned int VBO;
    unsigned int VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw our first triangle
        glUseProgram(shaderProgram);
        // create transformations
        // Camera
        // -----------------------------------


        // orbitalCamera/view transformation
        glm::mat4 view = orbitalCamera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // projection transformations
        glm::mat4 projection = glm::perspective(orbitalCamera.getFov(),  (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, /*(float)glfwGetTime() * */ glm::radians(150.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
        // glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);

        int lightLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3f(lightLoc, 1.0f, 1.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); // on met le nombre de point (36)
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
