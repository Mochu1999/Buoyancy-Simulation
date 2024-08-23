

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
	
	//21.77
	//19 after removing the lists
	TimeCounter tc;
	Lines3d trLines;
	AuxSquare auxSquare;
	vector<p3> spherePoints = addFibSphere(10000, 100);
	std::vector<p3> projectedPoints = stereographicProjection(spherePoints);


	vector<p> projectedPoints2d = p3ToP2(projectedPoints);

	vector<unsigned int> lidIndices, uselessVector;
	vector<Triangle> delaunay = bowyerWatson(projectedPoints2d, lidIndices);

	//printflat(lidIndices);

	vector<p> lidPoints;
	for (const auto& i : lidIndices)
	{
		//cout << "i: " << i << ", spherePoints[i]: {" << spherePoints[i].x << ", " << spherePoints[i].y << "}" << endl;
		lidPoints.push_back(p{ spherePoints[i].x,spherePoints[i].z });
		
	}

	//printv2(lidPoints);
	vector<Triangle> delaunay2 = bowyerWatson(lidPoints, uselessVector);
	
	//vector<p3> lid = p2ToP3(lidIndices);
	trLines.addSet(spherePoints);
	//auxSquare.addSet(spherePoints);

	trLines.indices = generateIndices(projectedPoints2d, delaunay);
	vector<unsigned int> secondIndices= generateIndices(lidPoints, delaunay2);
	trLines.indices.insert(trLines.indices.end(), secondIndices.begin(), secondIndices.end());

	tc.endCounter();

	/*printv3(trLines.positions);
	printflat(trLines.indices);*/

	





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





	Fourier fourier;
	fourier.updateWavePositions();

	//WettedSurface wettedSurface(polygon,fourier);




	//printvec3(test.positions);

	//getPos(window);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);





	GLint locationMVP = glGetUniformLocation(shader.m_RendererID, "u_MVP");
	GLint locationView = glGetUniformLocation(shader.m_RendererID, "u_ViewMatrix");
	int renderTypeLocation = glGetUniformLocation(shader.m_RendererID, "u_RenderType");



	

	





	int counter = 0;




	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);


	while (!glfwWindowShouldClose(window))
	{
		//system("cls");
		if (isRunning)
		{

			//polygon.translate({ 0,0.1,0 });
			//printv3(polygon.positions);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUniform1i(renderTypeLocation, 1);

			glUniform4f(shader.colorLocation, 1.0, 1.0, 1.0, 1.0);
			//groundLines.draw();

			glUniform4f(shader.colorLocation, 1.0, 0.0, 0.0, 1.0);
			xLine.draw();
			glUniform4f(shader.colorLocation, 0.0, 1.0, 0.0, 1.0);
			zLine.draw();
			glUniform4f(shader.colorLocation, 0.0, 0.0, 1.0, 1.0);
			yLine.draw();



			glUniform4f(shader.colorLocation, 1.0, 0.0, 1.0, 1);
			xLine2.draw();
			zLine2.draw();
			//wettedSurface.draw();
			//polygon.draw();
			//line.draw();

			//triangles.lines.draw();
			//triangles.draw();
			
			trLines.draw();
			auxSquare.draw();

			glUniform4f(shader.colorLocation, 135.0f / 255.0f, 0.0, 0.0, 1.0);

			

			glUniform1i(renderTypeLocation, 0);


			glUniform4f(shader.colorLocation, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.3f);
			//glUniform4f(shader.colorLocation, 00.0f / 255.0f, 204.0f / 255.0f, 0.0f / 255.0f, 0.1f);
			//printflat(fourier.indices);
			//fourier.createWavePositions();
			//fourier.draw();
			//newfourier.draw();
			
			glUniform1i(renderTypeLocation, 1);
			glUniform4f(shader.colorLocation, 0, 0, 0.0, 1.0);
			for (auto& line : fourier.lines)
			{
				//line.draw();
			}
			
			camera.updateCamera();
			glUniformMatrix4fv(locationMVP, 1, GL_FALSE, camera.vpMatrix.data());
			glUniformMatrix4fv(locationView, 1, GL_FALSE, camera.viewMatrix.data());



			//printp3(camera.forward);
			//printp3(camera.cameraPos);








			//glUniform4f(shader.colorLocation, 1.0f, 1.0f, 1.0f, 0.4f);
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


