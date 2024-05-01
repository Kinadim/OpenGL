/*
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec3 aNormal; 
out vec3 ourColor; // output a color to the fragment shader
out vec3 Normal; 
//out gl_Position;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
    Normal = mat3(transpose(inverse(model)))*aNormal;
    FragPos = vec3(model * vec4(aP // the color variable has attribute position 1os, 1.0));
    //rajoute la normal au tableau de vertices
}  
*/

#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal; 

out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 ObjColor;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    //ourColor = vec3(0.0f,0.0f,1.0f); //
    ourColor = ObjColor;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}