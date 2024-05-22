#pragma once

#include "Common.hpp"

struct Camera {

	float rotationSpeed = 0.05f;
	float translationSpeed = 0.3f;

	GLFWwindow* window;

	
	float fov = 45.0f * PI / 180;
	float aspectRatio = windowWidth / windowHeight;
	float nearZ = 0.1f;
	float farZ = 300.0f;

	std::array<float, 16> perspectiveMatrix;
	std::array<float, 16> viewMatrix;
	std::array<float, 16> vpMatrix;

	p3 cameraPos = { 45.1553,49.5914,132.395 };// { 50.0, 10.0, -10.0 };

	//p3 forward = { -1.0f,0.0f,1.0f };
	p3 forward = { 0.0304847,-0.556371,-0.828342 };
	p3 right; //0 because are gettin recalculated anyways
	p3 up;


	Camera(GLFWwindow* window_) :window(window_) {

		perspectiveMatrix = createPerspectiveMatrix();
	}




	
	////Perspective and View
	array<float, 16> createPerspectiveMatrix();

	std::array<float, 16> createViewMatrix(const p3& right, const p3& up, p3 forward, const p3& cameraPos);

	//the rotations create a new forward vector and the other 2 are deduced from it
	void calculateForward(p3& forward, const float rotationSpeed, const p3& rotationAxis);

	//takes the glfw inputs, executes the rotation and the translation, creates all the vector and updates the view and vp matrices
	void updateCamera();



	// auxiliary functions

	/*std::array<float, 4> createQuaternion2(float angle, const p3& axis);
	std::array<float, 4> inverseQuaternion2(const std::array<float, 4>& q);
	std::array<float, 4> multiplyQuaternions2(const std::array<float, 4>& a, const std::array<float, 4>& b);*/

	std::array<float, 16> multiplyMatrices(const std::array<float, 16>& a, const std::array<float, 16>& b);
};

