#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
//#define STB_IMAGE_IMPLEMENTATION
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glengine/orbitalCamera.hpp>
#include <stdio.h>
#include "projet/config.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

bool blinn = false;
char ombrage = 1;
// Taille de la fenêtre
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// Variables globales
// État de la camera
bool firstMouse = true;
float lastX;
float lastY;
bool tourne = false;

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


// Fonction pour gérer les entrées utilisateur
void processInput(GLFWwindow *window)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
      }
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        std::cout<<"patatra";
        //fragment_shader_file.open(std::string(_resources_directory).append("shaders/patatra.frag").c_str());
    }
}

void onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }
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

void computeNormal(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, std::vector<float>& normal) {
    // init normal vector
    for (size_t k = 0; k < vertices.size(); k++) {
        normal.push_back(0);
    }

    // Compute normal
    for (unsigned int k = 0; k < indices.size() - 3; k += 3) {
        unsigned int i1 = indices[k];
        unsigned int i2 = indices[k+1];
        unsigned int i3 = indices[k+2];

        // Vertices
        glm::vec3 v1 = glm::vec3(vertices[3*i1], vertices[3*i1 + 1], vertices[3*i1 + 2]);
        glm::vec3 v2 = glm::vec3(vertices[3*i2], vertices[3*i2 + 1], vertices[3*i2 + 2]);
        glm::vec3 v3 = glm::vec3(vertices[3*i3], vertices[3*i3 + 1], vertices[3*i3 + 2]);

        // Compute normal
        glm::vec3 n = glm::cross(v3 - v1, v3 - v2);

        for (size_t di = 0; di < 3; di++) {
            normal[3 * i1 + di] += n[di];
            normal[3 * i2 + di] += n[di];
            normal[3 * i3 + di] += n[di];
        }
    }

    // Normalize
    for (size_t k = 0; k < normal.size() - 3; k += 3) {
        glm::vec3 n = glm::normalize(glm::vec3(normal[k], normal[k + 1], normal[k + 2]));
        normal[k] = n[0];
        normal[k+1] = n[1];
        normal[k+2] = n[2];
    }
}

std::vector<float> fusionne(const std::vector<float>& vertices, const std::vector<float>& normal){
    std::vector<float> vertimal;
    for(size_t i=0; i<vertices.size();i+=3){
        vertimal.push_back(vertices[i]);
        vertimal.push_back(vertices[i+1]);
        vertimal.push_back(vertices[i+2]);
        vertimal.push_back(normal[i]);
        vertimal.push_back(normal[i+1]);
        vertimal.push_back(normal[i+2]);
    }
    return vertimal;
}
struct ObjectData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
};

ObjectData lis_et_remplis2() {
    ObjectData objectData;

    // Code de l'objet
    std::string objet_code_v;
    std::string objet_code_f;

    // Ouverture du fichier
    std::ifstream objet_file;
    objet_file.open(std::string(_resources_directory).append("objets/dragon2_small.obj").c_str());

    // Lecture du fichier
    std::stringstream objet_iss;

    if (objet_file.is_open()) {
        std::string ligne;

        while (std::getline(objet_file, ligne)) {
            // Vérifier les lignes commençant par '#' ou contenant '...'
            if (ligne.size() > 0 && (ligne[0] == '#' || ligne.find("...") != std::string::npos)) {
                continue; // Sauter les lignes commençant par '#' ou contenant '...'
            }
            if(ligne[0] == 's'){
                continue;
            }

            // Vérifier si la ligne commence par 'f' ou 'v' et l'ajouter respectivement
            if (ligne.size() > 0 && ligne[0] == 'f') {
                ligne = ligne.erase(0, 2);
                objet_code_f += ligne + "\n";
            } else if (ligne.size() > 0 && ligne[0] == 'v') {
                ligne = ligne.erase(0, 2);
                objet_code_v += ligne + "\n";
                //std::cout<<"objet_code_v : "<<objet_code_v<<std::endl;
            }

            objet_iss << ligne << std::endl;
        }

        objet_file.close();
    } else {
        std::cerr << "Impossible d'ouvrir le fichier : " << _resources_directory << "objets/dragon2_small.obj" << std::endl;
        // Gestion de l'erreur ici (vous pouvez modifier selon vos besoins)
        return objectData;
    }

    std::istringstream ss(objet_code_v);
    std::string element;

    while (ss >> element) {
        if (element != " " && element != "\n") {
            //std::cout<<"element : "<<element<<std::endl;
            objectData.vertices.push_back(std::stof(element));
        }
    }

    std::istringstream ss2(objet_code_f);
    unsigned int indice;

    while (ss2 >> indice) {
        if (element != " " && element != "\n") {
        //std::cout<<"indice : "<<indice<<std::endl;
        objectData.indices.push_back(indice-1);
        }
    }

    std::cout << std::endl << "+++++++++++++++++++++++++++++" << std::endl;
    std::cout << std::endl << "-----------------------------" << std::endl;
    std::cout << "Nombre d'éléments dans vertices : " << objectData.vertices.size() << std::endl;
    std::cout << "Nombre d'éléments dans indices : " << objectData.indices.size() << std::endl;

    computeNormal(objectData.vertices,objectData.indices,objectData.normals);
    objectData.vertices= fusionne(objectData.vertices,objectData.normals);
    return objectData;
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Projet", NULL, NULL);
    if (window == NULL)
        {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
        }

    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onMouseMove);    
    glfwMakeContextCurrent(window);             // sans cette ligne pas de glad


    // Initialise ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

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

    ObjectData result = lis_et_remplis2();
    std::vector<float> vertices = result.vertices;
    std::vector<unsigned int> indices = result.indices;
    std::vector<float> normales = result.normals;


// Création et configuration des buffers
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW); 

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0);

    // Normales
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, normales.size()*sizeof(float), normales.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);  
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(0);
    
    float lumosX = 1.2f;
    float lumosY = 1.0f;
    float lumosZ = 2.0f;
    float ambientStrength = 0.5f;
    float shininess = 200.0f;
    float gaussian = 0.05f;


     glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f); // Initial color (rouge)
     glm::vec3 lumos = glm::vec3(1.0f, 1.0f, 1.0f); // Initial color (blanc)
    // Boucle principale de rendu
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        // Gestion des entrées utilisateur
        processInput(window);  


        // Nouveau cadre ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        ImGui::Text("Hello, worldd!");

        if (ImGui::Button("Phong"))
        {
            ombrage = 1;// Code exécuté lors du clic sur le bouton

        }
        else if (ImGui::Button("Blinn-Phong"))
        {
            ombrage = 2;// Code exécuté lors du clic sur le bouton

        }
        else if (ImGui::Button("Gaussien"))
        {
            ombrage = 3;// Code exécuté lors du clic sur le bouton

        }
        if(ImGui::Button("Tourner l'objet")){
            tourne=!tourne;
        }
        switch(ombrage){
            case 1:
            ImGui::Text("Phong");
            break;
         case 2:
            ImGui::Text("Blinn-Phong");
            break;
         case 3:
            ImGui::Text("Gaussien");
            break;
         default:
            ImGui::Text("Phong");
      }
      /*
        if(blinn){
            ImGui::Text("Blinn-Phong");
        }
        else{
            ImGui::Text("Phong");
        }
        */

        if(ImGui::CollapsingHeader("Position Lumière")){
        ImGui::SliderFloat("LumièreX", &lumosX,-50.0f,50.0f);
        ImGui::SliderFloat("LumièreY", &lumosY,-50.0f,50.0f);
        ImGui::SliderFloat("LumièreZ", &lumosZ,-50.0f,50.0f);
        }
        //ImGui::ColoEdit3("Couleur",);

        if(ImGui::CollapsingHeader("Couleur")){
              // Example: Color Picker
            
            ImGui::ColorEdit3("Color Object", (float*)&color);
            ImGui::ColorEdit3("Color Lumière", (float*)&lumos);  
        }

        if(ImGui::CollapsingHeader("Coefficient")){
              // Example: Color Picker
            ImGui::SliderFloat("Force Ambiente", &ambientStrength,-1.0f,1.0f);

            
            ImGui::SliderFloat("Brillance", &shininess,1.0f,300.0f);
            ImGui::SliderFloat("Coeff Gausian", &gaussian,0.0f,1.0f); 
        }

        ImGui::Render();
        ImGui::EndFrame();
       

        // Effacement du tampon de couleur et du tampon de profondeur
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Utilisation du programme de shaders
        glUseProgram(shaderProgram);
/*
        // ... (Code pour définir les matrices de transformation)
        glm::mat4 view       = glm::mat4(1.0f); 
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 model      = glm::mat4(1.0f);

        ////////projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

*/

        // Camera
        // -----------------------------------


        // orbitalCamera/view transformation
        glm::mat4 view = orbitalCamera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // projection transformations
        glm::mat4 projection = glm::perspective(orbitalCamera.getFov(),  (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        if(tourne){
        model = glm::rotate(model, /*(float)glfwGetTime() * */ glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        }
        else if(!tourne){
        model = glm::rotate(model, (float)glfwGetTime() *  glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        }

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        
        //glUniform1i(glGetUniformLocation(shaderProgram,"blinn", blinn ));
        
        int ombrageLoc = glGetUniformLocation(shaderProgram,"ombrage");
        glUniform1i(ombrageLoc, ombrage);
    ///////////////////////////////////////////////////////////////////////////////////////////////
        int AmbientePower = glGetUniformLocation(shaderProgram,"ambientStrength");
        glUniform1f(AmbientePower, ambientStrength);

        int BrillancePower = glGetUniformLocation(shaderProgram,"shininess");
        glUniform1f(BrillancePower, shininess);

        int GausianLoc = glGetUniformLocation(shaderProgram,"gaussian");
        glUniform1f(GausianLoc,gaussian);

        
    ///////////////////////////////////////////////////////////////////////////////////////////////
        glm::vec3 lightColor(lumos);
        int lightCouleur = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightCouleur, 1, &lightColor[0]);

        glm::vec3 lightLoc(lumosX,lumosY,lumosZ);
        int lightPosition = glGetUniformLocation(shaderProgram, "lightLoc");
        glUniform3fv(lightPosition, 1,&lightLoc[0]);

        glm::vec3 objColor(color);
        int objetColor = glGetUniformLocation(shaderProgram, "ObjColor");
        glUniform3fv(objetColor,  1, &objColor[0]);

///////////METTRE LA LUMIÈRE AU MEME ENDROIT QUE LA CAMÉRA///////////////////////////////////////////////
        //glUniform3fv(lightPosition, 1,glm::value_ptr(orbitalCamera.getPosition()));
        

        
        glm::vec3 viewPosition(1.0f, 1.0f, 1.0f);
        int viewPos = glGetUniformLocation(shaderProgram, "viewLoc");
        glUniform3fv(viewPos, 1, &viewPosition[0]);


        // Liaison du tableau de vertices
        glBindVertexArray(VAO);
        // Rendu des vertices
        //glDrawArrays(GL_TRIANGLES, 0, 36);         //on met le nombre de point (36)
        glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time

        // Échange des tampons et gestion des événements
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // ... (Nettoyage des ressources)

ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplGlfw_Shutdown();
ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    std::cout << "Hello worldd !" << std::endl;
    return 0;
}

