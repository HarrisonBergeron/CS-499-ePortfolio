#version 440 core
layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec4 color; 
layout(location = 2) in vec2 tex; 
layout(location = 3) in vec3 normal; 
out vec4 vertexColor; 
out vec2 vertexTex; 
out vec3 vertexNormal; 
out vec3 vertexFragPos; 

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

void main() 
{ 
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
    vertexColor = color; 
    vertexTex = tex; 

    vertexNormal = mat3(transpose(inverse(model))) * normal; 

    vertexFragPos = vec3(model * vec4(aPos, 1.0f)); 
}