

#include "Common.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include "lines3d.hpp"
#include "Fourier.hpp"

#include "Camera.hpp"

#include "auxiliary_elements.h"

#include "BinariesManager.h"
#include "KeyMouseImputs.hpp"

#include "Polygons.hpp"

#include "WettedSurface.hpp"

#include "AuxSquare.hpp"

#include "Spheres.hpp"

#include "Delaunay_2D.hpp"
#include "Arrows.hpp"
#include "Pyramid.hpp"


// to not render what is not visible to the camera:
//glEnable(GL_CULL_FACE);       // Enable face culling
//glCullFace(GL_BACK);          // Cull back faces
//glFrontFace(GL_CCW);          // Counter-clockwise winding is front-facing




using namespace std::chrono;

float xpos, ypos;
double xpos1, ypos1;

struct AllPointers {
	BinariesManager* binariesManager;
	Polygons* polygon;
};



void getPos(GLFWwindow* window) {
	glfwGetCursorPos(window, &xpos1, &ypos1);
	ypos1 = windowHeight - ypos1;
	xpos = (float)xpos1;
	ypos = (float)ypos1;
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_P:
			isRunning = !isRunning;
			break;
		}
	}
}



int main(void)
{
	GLFWwindow* window = initialize();

	/*glDebugMessageCallback(MessageCallback, nullptr);
	glEnable(GL_DEBUG_OUTPUT);*/


	BinariesManager binariesManager;

	model = { {25,0,25},{45,0,25} ,{45,20,25},{35,30,25} ,{25,20,25},{25,0,25} };
	//printv3(model);

	Auxiliary_Elements groundLines;


	Shader shader("resources/shaders/shader1.shader");
	shader.Bind();


	Camera camera(window);

	Pyramid pyramid;

	////////////////////////////////////////////////////////////
	//Sphere sphere(50, 100);

	//sphere.addSet({ 00,00,00 });

	//vector<p3> centroids;
	//for (unsigned int i = 0; i < sphere.indices.size(); i += 3)
	//{
	//	centroids.push_back(
	//		centroid(sphere.positions[sphere.indices[i]], sphere.positions[sphere.indices[i + 1]], sphere.positions[sphere.indices[i + 2]])
	//	);
	//}
	//Arrows arrows;
	//arrows.addSet(centroids, sphere.normals);





	Lines3d xLine2;
	xLine2.addSet({ {0,0,0},{5,0,0} });
	Lines3d xLine;
	xLine.addSet({ {-100,0,0},{100,0,0} });
	Lines3d yLine;
	yLine.addSet({ {0,-100,0},{0,100,0} });
	Lines3d zLine;
	zLine.addSet({ {0,0,-100},{0,0,100} });
	Lines3d zLine2;
	zLine2.addSet({ {0,0,0},{0,0,5} });

	Polygons polygon;
	polygon.addSet(model);





	//Fourier fourier;
	//fourier.updateWavePositions();

	//WettedSurface wettedSurface(polygon,fourier);









	//getPos(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLint locationMVP = glGetUniformLocation(shader.ID, "u_MVP");
	GLint locationView = glGetUniformLocation(shader.ID, "u_ViewMatrix");
	int renderTypeLocation = glGetUniformLocation(shader.ID, "u_RenderType");
	int colorLocation = glGetUniformLocation(shader.ID, "u_Color");
	int cameraPosition = glGetUniformLocation(shader.ID, "camPos");









	int counter = 0;




	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);


	while (!glfwWindowShouldClose(window))
	{
		break;
		//system("cls");
		if (isRunning)
		{
			

			glClearColor(0.035f, 0.065f, 0.085f, 1.0f);
			//polygon.translate({ 0,0.1,0 });
			//printv3(polygon.positions);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUniform1i(renderTypeLocation, 1);

			glUniform4f(colorLocation, 1.0, 1.0, 1.0, 1.0);
			//groundLines.draw();

			glUniform4f(colorLocation, 1.0, 0.0, 0.0, 1.0);
			xLine.draw();
			glUniform4f(colorLocation, 0.0, 1.0, 0.0, 1.0);
			yLine.draw();
			glUniform4f(colorLocation, 0.0, 0.0, 1.0, 1.0);
			zLine.draw();



			glUniform4f(colorLocation, 1.0, 0.0, 1.0, 1);
			xLine2.draw();
			zLine2.draw();
			//wettedSurface.draw();
			polygon.draw();
			//line.draw();

			//triangles.lines.draw();
			//triangles.draw();

			/*trLines.draw();
			auxSquare.draw();*/







			glUniform1i(renderTypeLocation, 0);
			glUniform4f(colorLocation, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
			glUniform4f(colorLocation, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			//printflat(fourier.indices);
			//fourier.createWavePositions();
			//fourier.draw();
			//sphere.draw();
			//pyramid.draw();

			glUniform1i(renderTypeLocation, 1);
			glUniform4f(colorLocation, 0, 0, 1.0, 1.0);
			//for (auto& line : fourier.lines)
			//{
			//	line.draw();
			//}
			//arrows.draw();

			glUniform4f(colorLocation, 187.0f / 255.0f, 165.61f / 255.0f, 61.0f / 255.0f, 1);

			glUniform4f(colorLocation, 135.0f / 255.0f, 0.0, 0.0, 1);
			//sphere.drawLines();
			//sphere.draw();

			camera.updateCamera();
			//glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
			glUniform3f(cameraPosition, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
			glUniformMatrix4fv(locationMVP, 1, GL_FALSE, camera.vpMatrix.data());
			glUniformMatrix4fv(locationView, 1, GL_FALSE, camera.viewMatrix.data());



			//printp3(camera.forward);
			//printp3(camera.cameraPos);








			//glUniform4f(colorLocation, 1.0f, 1.0f, 1.0f, 0.4f);
			//groundLines.draw();//anisotropic filtering




		}
		//isRunning = false;

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


