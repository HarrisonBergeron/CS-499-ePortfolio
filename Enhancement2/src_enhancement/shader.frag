#version 440 core
in vec4 vertexColor; 
in vec2 vertexTex; 
in vec3 vertexNormal; 
in vec3 vertexFragPos; 
out vec4 FragColor; 
uniform sampler2D uTexture; 

void main() 
{ 
    vec3 lightColor = vec3(1.5, 1.5, 1.0); 
    vec3 lightDir = vec3(2.0, -1.5, -1.0); 

    float impact = max(dot(normalize(vertexNormal), normalize(-lightDir)), 0.0); 
    vec3 diffuse = impact * lightColor;

    float ambientStrength = 0.4; 
    vec3 ambient = ambientStrength * vec3(0.8, 0.8, 1.0); 

    vec3 phong = ambient + diffuse; 

    FragColor = vec4(phong, 1.0) * vertexColor * texture(uTexture, vertexTex); 
}