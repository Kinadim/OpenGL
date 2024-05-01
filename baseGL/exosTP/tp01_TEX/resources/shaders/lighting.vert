#version 330 core
layout (location = 0) in vec3 aPos;     // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;  // the normal variable has attribute position 1
layout (location = 2) in vec3 aTexture; // the texture variable has attribute position 2
out vec3 ourTexture; // output a color to the fragment shader
out vec3 Normal; 
//out gl_Position;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightLoc; 

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourTexture = aTexture; // set ourColor to the input color we got from the vertex data
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    //rajoute la normal au tableau de vertices
}  
