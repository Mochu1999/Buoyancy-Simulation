#pragma once

#include "Common.hpp"



struct Camera {

	float rotationSpeed = 0.05f;
	float translationSpeed = 0.3f;

	GLFWwindow* window;

	
	float fov = 45.0f * PI / 180;
	float aspectRatio = windowWidth / windowHeight;
	float nearZ = 0.1f;
	float farZ = 3000.0f;

	std::array<float, 16> perspectiveMatrix;
	std::array<float, 16> viewMatrix;
	std::array<float, 16> vpMatrix;

	std::array<float, 16> orthoMatrix;

	p3 cameraPos;

	//p3 forward = { 0.0f,-1.0f,0.0f }; //cuando dejó esto de funcionar?
	p3 forward = normalize3(p3{ 0.148691,-0.742031,-0.651083 });
	p3 right; //0 because are gettin recalculated anyways
	p3 up;


	Camera(GLFWwindow* window_) :window(window_) {

		orthoMatrix = createOrthoMatrix();
		perspectiveMatrix = createPerspectiveMatrix();
	}




	array<float, 16> createOrthoMatrix();

	////Perspective and View
	array<float, 16> createPerspectiveMatrix();

	std::array<float, 16> createViewMatrix(const p3& right, const p3& up, p3 forward, const p3& cameraPos);

	//the rotations create a new forward vector and the other 2 are deduced from it
	void calculateForward(p3& forward, const float rotationSpeed, const p3& rotationAxis);

	//takes the glfw inputs, executes the rotation and the translation,
	void updateKeys();

	//creates all the vector and updates the view and vp matrices
	void updateCamera();





	float xpos, ypos;
	double xpos1, ypos1;

	void getPos(GLFWwindow* window) {
		glfwGetCursorPos(window, &xpos1, &ypos1);
		ypos1 = windowHeight - ypos1;
		xpos = (float)xpos1;
		ypos = (float)ypos1;
	}
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);