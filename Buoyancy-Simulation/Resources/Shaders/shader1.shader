#shader vertex 
#version 330 core

// Assigns the slots in the Vertex Attribute Array where these attributes (position and normal) go
layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal;

//uniforms are universal variables that can be accessed from other shaders and in main without the use of a VAO
uniform mat4 u_MVP;
uniform mat4 u_OrthoProjection;

uniform int u_3d;

out vec3 Normal;  //Output that goes to the fragment shader
out vec3 crntPos;


void main() {
	if (u_3d == 1) //3d
	{
		crntPos = aPos;
		Normal = aNormal;
		gl_Position = u_MVP * vec4(crntPos,1.0); //gl_Position is a keyword
	}
	else if (u_3d == 0) //2d
	{
        gl_Position = u_OrthoProjection * vec4(aPos.xy, 0.0, 1.0);
    }
    
}




#shader fragment
#version 330 core


out vec4 FragColor;

in vec3 Normal;
in vec3 crntPos;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform int u_RenderType;
uniform vec3 camPos;
uniform vec3 u_lightPos;


uniform vec4 u_Color;

void main()
{
	if (u_RenderType == 0) 
	{
		float ambient = 0.35f;
		float specularLight = 0.7f;

		// diffuse lighting
		vec3 normal = normalize(Normal);
		vec3 lightDirection = normalize(u_lightPos - crntPos);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		// specular lighting
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
		float specular = specAmount * specularLight*1.2;

		// outputs final color
		FragColor =  u_Color * lightColor * (diffuse + ambient + specular);
	}
	if (u_RenderType == 1) 
    {
        FragColor = u_Color;
    }
}



