#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 ourColor;  

uniform vec3 lightLoc; 
uniform vec3 viewLoc; 
uniform vec3 lightColor;
uniform int ombrage;
uniform float ambientStrength;
uniform float shininess;
uniform float gaussian;


void main()
{
    if(ombrage == 1){
        // ambient
        vec3 ambient = ambientStrength * lightColor;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightLoc - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewLoc - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;  
            
        vec3 result = (ambient + diffuse + specular) * ourColor;
        FragColor = vec4(result, 1.0);
    }
    else if(ombrage ==2){
            // ambient
            vec3 ambient = ambientStrength * lightColor;
            
            // diffuse 
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightLoc - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            // specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewLoc - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = 0.0;
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
            vec3 specular =  spec * lightColor;  // assuming bright white light color
            FragColor = vec4((ambient + diffuse + specular)*ourColor, 1.0);
    }
    else if(ombrage ==3){

        //ambient
        vec3 ambient = lightColor * ambientStrength  ;

        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightLoc - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        //specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewLoc - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm,halfwayDir),0.0),2.0f*gaussian);

        //gaussian
        float cosAngIncidence = dot(norm, lightDir);
        cosAngIncidence = clamp(cosAngIncidence, 0.0, 1.0);
        float angleNormalHalf = acos(dot(halfwayDir, norm));
        float exponent = angleNormalHalf / gaussian;
        exponent = -(exponent *  exponent);
        float gaussianTerm = exp(exponent);

        gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;

        vec3 specular = specularStrength * spec * lightColor;

        FragColor = vec4((ambient + diffuse + specular)*ourColor, 1.0);


    }
    
}