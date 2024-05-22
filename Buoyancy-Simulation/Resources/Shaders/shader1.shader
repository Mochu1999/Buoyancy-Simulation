#shader vertex 
#version 330 core

layout(location = 0) in vec4 position; 
layout(location = 1) in vec3 normal;  // Add this line to input vertex normals

uniform mat4 u_MVP;
uniform mat4 u_ViewMatrix; // Add a uniform for the model-view matrix

out vec3 fragNormal;  // Pass the normal to the fragment shader

void main() {
    fragNormal = mat3(u_ViewMatrix) * normal; // Transform the normal
    gl_Position = u_MVP * position;
}




#shader fragment
#version 330 core
layout(location = 0) out vec4 color;
in vec3 fragNormal;  // Receive the normal from the vertex shader

uniform vec4 u_Color;
 vec3 lightDirection = vec3(0.0, -1.0, 0.0);

 float ambientStrength = 0.0; // Ambient light intensity

 uniform int u_RenderType; // 0 for geometry without normals, 1 for geometry with normal
  
void main() {

    if (u_RenderType == 0) 
    {
        vec3 normal = normalize(fragNormal);

        float diffuse = max(dot(normal, -lightDirection), 0.0);


        vec4 ambient = ambientStrength * u_Color;
        color = (ambient + diffuse)*2 * u_Color;
        //color = diffuse * u_Color;
    }
    if (u_RenderType == 1) 
    {
        color = u_Color;
    }
}