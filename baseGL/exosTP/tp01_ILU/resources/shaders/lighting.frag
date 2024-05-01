#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightLoc; 
uniform vec3 lightColor;
uniform vec3 result;
uniform vec3 viewLoc;
  
void main()
{
    //ambient
    float ambientStrength = 0.5;
    float specularStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightLoc - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewLoc - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 200);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse+  specular) * ourColor;
    FragColor = vec4(result, 1.0);
/*
    bool blinn = false;
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4((ambient + diffuse + specular)*ourColor, 1.0);
*/
}