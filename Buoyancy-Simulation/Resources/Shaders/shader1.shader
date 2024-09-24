#shader vertex 
#version 330 core

// Assigns the slots in the Vertex Attribute Array where these attributes (position and normal) go
layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal;

//uniforms are universal variables that can be accessed from other shaders and in main without the use of a VAO
uniform mat4 u_MVP;


out vec3 Normal;  //Output that goes to the fragment shader
out vec3 crntPos;


void main() {
    crntPos = aPos;
    Normal = aNormal;
     gl_Position = u_MVP * vec4(crntPos,1.0); //gl_Position is a keyword
}




#shader fragment



#version 330 core


out vec4 FragColor;

in vec3 Normal;
in vec3 crntPos;

//flat in vec3 Normal;  // Ensure you use flat shading for normals

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
vec3 lightPos = vec3(30,50,25);

uniform int u_RenderType;
uniform vec3 camPos;
uniform vec4 u_Color;

vec4 objectColor = vec4(0.0f,0.0f,1.0f,1.0f);

void main()
{
	if (u_RenderType == 0) 
	{
		float ambient = 0.35f;
		float specularLight = 0.7f;

		// diffuse lighting
		vec3 normal = normalize(Normal);
		vec3 lightDirection = normalize(lightPos - crntPos);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		// specular lighting
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
		float specular = specAmount * specularLight*1.2;

		// outputs final color
		FragColor =  objectColor * lightColor * (diffuse + ambient + specular);
	}
	if (u_RenderType == 1) 
    {
        FragColor = u_Color;
    }
}



