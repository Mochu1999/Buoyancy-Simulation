#include "Camera.hpp"

array<float, 16> Camera::createPerspectiveMatrix() {
	array<float, 16> perspectiveMatrix = {};

	float tanHalfFov = tan(fov / 2);
	
	perspectiveMatrix[0] = 1 / (aspectRatio * tanHalfFov);
	perspectiveMatrix[5] = 1 / (tanHalfFov);
	perspectiveMatrix[10] = -(farZ + nearZ) / (farZ - nearZ);
	perspectiveMatrix[11] = -1.0f;
	perspectiveMatrix[14] = -2 * farZ * nearZ / (farZ - nearZ);

	return perspectiveMatrix;
}

//we want to move the world so the camera frustrum ends clipped in the square of range -1,1 for the axes x and y; and same range for the
// near and far z planes
std::array<float, 16> Camera::createViewMatrix(const p3& right, const p3& up, p3 forward, const p3& cameraPos) {



	std::array<float, 16> viewMatrix = { 0 };

	//rotation: change of basis from the world axes to one aligned with the camera local axes right, up and forward respectlively
	//the forward element is negative because we consider -z to be forward
	viewMatrix[0] = right.x;
	viewMatrix[1] = up.x;
	viewMatrix[2] = -forward.x;

	viewMatrix[4] = right.y;
	viewMatrix[5] = up.y;
	viewMatrix[6] = -forward.y;

	viewMatrix[8] = right.z;
	viewMatrix[9] = up.z;
	viewMatrix[10] = -forward.z;

	//Translation. Moves the world en the opposite direction of the camera position so it ends up in the origin
	// It must be done with a dot product. Because otherwise when the camera's rotated, you wouldn't end in the origin because the camera
	// would be translated with respect of its local axes
	viewMatrix[12] = -dot3(right, cameraPos);
	viewMatrix[13] = -dot3(up, cameraPos);
	viewMatrix[14] = dot3(forward, cameraPos);


	viewMatrix[15] = 1.0f;

	return viewMatrix;
}

void Camera::calculateForward(p3& forward, const float rotationSpeed, const p3& rotationAxis) {
	std::array<float, 4> q, q_inv, p, interm1, interm2;
	p3 intermForward;

	q = createQuaternion(rotationSpeed, rotationAxis);

	q_inv = inverseQuaternion(q);
	p = { 0, forward.x, forward.y, forward.z };

	interm1 = multiplyQuaternions(q, p);
	interm2 = multiplyQuaternions(interm1, q_inv);

	intermForward = normalize3(p3{ interm2[1], interm2[2], interm2[3] });

	if (abs(intermForward.y) < 0.99)
		forward = intermForward;
}

void Camera::updateCamera() {

	// Rotation
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		calculateForward(forward, rotationSpeed, right);

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		calculateForward(forward, -rotationSpeed, right);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		calculateForward(forward, -rotationSpeed, up);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		calculateForward(forward, rotationSpeed, up);


	//translation
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos = cameraPos + forward * translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos = cameraPos - forward * translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos = cameraPos - right * translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos = cameraPos + right * translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos.y += translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos.y -= translationSpeed;


	//The quaternion method is intentionally incomplete. The true method would calculate the f and u for pitch and f and r for yaw
	// But instead of calculating everything there we are only calculating f and here forcing right to be with respect of the referenceUp
	// of the world and up is calculated from there
	right = normalize3(cross3(forward, { 0,1,0 }));
	up = cross3(right, forward);

	viewMatrix = createViewMatrix(right, up, forward, cameraPos);

	vpMatrix = multiplyMatrices(perspectiveMatrix, viewMatrix);

}



std::array<float, 16> Camera::multiplyMatrices(const std::array<float, 16>& a, const std::array<float, 16>& b) {
	std::array<float, 16> output = {};

	for (int i = 0; i < 4; ++i) { // row index
		for (int j = 0; j < 4; ++j) { // column index
			output[j * 4 + i] =
				a[0 * 4 + i] * b[j * 4 + 0] +
				a[1 * 4 + i] * b[j * 4 + 1] +
				a[2 * 4 + i] * b[j * 4 + 2] +
				a[3 * 4 + i] * b[j * 4 + 3];
		}
	}

	return output;
}


