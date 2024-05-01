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
}