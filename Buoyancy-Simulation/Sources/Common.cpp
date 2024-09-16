#include "Common.hpp"



//Variables
p cursor = { 0, 0 };
float windowHeight = 1080;
float windowWidth = 1920;
bool isRunning = true;


float fastInverseSqrt(float number) {
	long i;
	float x2, y;
	x2 = number * 0.5F;
	y = number;
	std::memcpy(&i, &y, sizeof(i)); // Safer type-punning
	i = 0x5f3759df - (i >> 1);
	std::memcpy(&y, &i, sizeof(y)); // Safer type-punning
	y = y * (1.5f - (x2 * y * y));
	return y;
}

void getCursorPos(GLFWwindow* window) {

	double intermX, intermY;

	glfwGetCursorPos(window, &intermX, &intermY);

	cursor = { static_cast<float>(intermX), static_cast<float>(windowHeight - intermY) };

}

float isRightOfLine(p& A, p& B, p& P) {		 //is P to the right of AB?
	p AB = { B.x - A.x,  B.y - A.y };
	p AP = { P.x - A.x, P.y - A.y };

	return AB.x * AP.y - AB.y * AP.x;	//if negative it is to its right, if 0, P is on the infinite line of AB
}

std::array<float, 4> createQuaternion(float angle, const p3& axis) {
	std::array<float, 4> q;
	float s = sin(angle / 2);
	q[0] = cos(angle / 2); // w
	q[1] = axis.x * s; // x
	q[2] = axis.y * s; // y
	q[3] = axis.z * s; // z
	return q;
}

std::array<float, 4> inverseQuaternion(const std::array<float, 4>& q) {
	return { q[0], -q[1], -q[2], -q[3] }; // q^-1 = [w, -x, -y, -z] for a unit quaternion
}

std::array<float, 4> multiplyQuaternions(const std::array<float, 4>& a, const std::array<float, 4>& b) {
	std::array<float, 4> result;
	result[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3]; // w
	result[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2]; // x
	result[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1]; // y
	result[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0]; // z
	return result;
}

// q * p * q^-1
p3 rotatePoint(const p3& point, const std::array<float, 4>& rotationQuaternion) {

	std::array<float, 4> pointQuat = { 0, point.x, point.y, point.z };
	std::array<float, 4> inverseQuat = inverseQuaternion(rotationQuaternion);


	std::array<float, 4> rotatedPointQuat = multiplyQuaternions(multiplyQuaternions(rotationQuaternion, pointQuat), inverseQuat);

	return p3(rotatedPointQuat[1], rotatedPointQuat[2], rotatedPointQuat[3]);
}

float isBelowTriangle(const p3& a, const p3& b, const p3& c, const p3& p) {

	//equivalent to the signed volume of a tetrahedron without the /6.0f
	return dot3(cross3(b - a, c - a), p - a);

}


void printm16_without_macro(const std::string& name, const std::array<float, 16>& matrix)
{
	std::stringstream ss;
	ss << name << " = " << std::endl;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			// For column-major order, access the matrix elements in this order:
			ss << matrix[col * 4 + row] << " ";
		}
		ss << std::endl; // Newline at the end of each row
	}

	std::cout << ss.str() << std::endl;
}