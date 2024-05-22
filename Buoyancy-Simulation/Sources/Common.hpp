#pragma once

//common file that does not depend on other files



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cmath>
#include <numeric> 
#include <algorithm>

#include <array>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <list>
#include <deque>

#include <chrono>
using namespace std;
using namespace std::chrono;

//not templated definitions of functions need to be separated from their declarations


template<typename T>
struct vec2 {
	T x, y;

	vec2() : x(0), y(0) {} //initializing paramenters to 0 if not specified 

	vec2(T x, T y) : x(x), y(y) {} //normal constructor

	// operator overloads
	bool operator == (const vec2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const vec2& other) const {
		return !(*this == other);
	}

	vec2 operator + (const vec2& other) const {
		return { x + other.x, y + other.y };
	}

	vec2 operator - (const vec2& other) const {
		return { x - other.x, y - other.y };
	}

	vec2 operator *(T scalar) const {
		return { x * scalar, y * scalar};
	}

	vec2 operator /(T scalar) const {
		return { x / scalar, y / scalar };
	}

	T dot(const vec2& other) const {
		return x * other.x + y * other.y;
	}

	T cross(const vec2& other) const {
		return x * other.y - y * other.x;
	}
};

using p = vec2<float>;
using ui2 = vec2<unsigned int>;



template<typename T>
struct vec3 {
	T x, y, z;

	vec3() : x(0), y(0), z(0) {} //initializing paramenters to 0 if not specified 

	vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {} //normal constructor

	// operator overloads
	bool operator == (const vec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const vec3& other) const {
		return !(*this == other);
	}

	vec3 operator + (const vec3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	vec3 operator - (const vec3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}

	vec3 operator *(T scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	vec3& operator += (const vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3& operator -= (const vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

};

using p3 = vec3<float>;
using ui3 = vec3<unsigned int>;


//these must be out of vec3 be accesible without the need of calling them like variable.cross()

//sum of products
template<typename T>
T dot3(const vec3<T>& v1, const vec3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
vec3<T> cross3(const vec3<T>& v1, const vec3<T>& v2) {
	return vec3<T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

float fastInverseSqrt(float number);

template<typename T>
vec3<T> normalize3(const vec3<T>& v) {
    float magnitudeSquared = v.x * v.x + v.y * v.y + v.z * v.z;
    if (magnitudeSquared == 0.0f) {
        return vec3<T>(0, 0, 0); // Return a zero vector to avoid division by zero
    }
    float magnitude_inv = fastInverseSqrt(magnitudeSquared);
    return vec3<T>(v.x * magnitude_inv, v.y * magnitude_inv, v.z * magnitude_inv);
}

template<typename T>
float magnitude3(const vec3<T>& v) {
	float magnitudeSquared = v.x * v.x + v.y * v.y + v.z * v.z;
	return sqrt(magnitudeSquared);
}






// Declarations of global variables
extern p cursor;
extern float windowHeight;
extern float windowWidth;
extern bool isRunning;
inline vector<p3> model;// = { {25,0,25},{25 + 20,0,25} ,{25 + 20,20,25} ,{25,20,25},{25,0,25} };

// Constants
constexpr double PI = 3.14159265358979323846;
constexpr double halfPI = PI/2;




void getCursorPos(GLFWwindow* window);

#define print(var) print_without_macro(#var, var)
template<typename T>
void print_without_macro(const string& name, const T& item) {
	stringstream ss;
	cout << name << ": " << item << endl;
}


#define printv2(var) printv2_without_macro(#var, var)
template<typename T>
void printv2_without_macro(const string& name, const vector<vec2<T>>& items) {
	stringstream ss;
	ss << name << endl;
	for (size_t i = 0; i < items.size(); ++i) {
		ss << "{" << items[i].x << "," << items[i].y << "}";

		if (i != items.size() - 1) { // Check if it's not the last element
			ss << ",";
		}
	}
	cout << ss.str() << endl << endl;
}

#define printv3(var) printv3_without_macro(#var, var)
template<typename T>
void printv3_without_macro(const string& name, const vector<vec3<T>>& items) {
	stringstream ss;
	ss << name << endl;
	for (size_t i = 0; i < items.size(); ++i) {
		ss << "{" << items[i].x << "," << items[i].y << "," << items[i].z << "}";

		if (i != items.size() - 1) { // Check if it's not the last element
			ss << ",";
		}
	}
	cout << ss.str() << endl << endl;
}

#define printflat(var) printflat_without_macro(#var, var)
template<typename T>
void printflat_without_macro(const string& name, const vector<T>& items) {
	stringstream ss;
	ss << name << endl;
	ss << "{";
	for (size_t i = 0; i < items.size(); i++) {
		ss << items[i];

		if (i != items.size() - 1) { // Check if it's not the last element
			ss << ",";
		}
	}
	ss << "}";
	cout << ss.str() << endl << endl;
}

#define printp3(var) printp3_without_macro(#var, var)
template<typename T>
void printp3_without_macro(const string& name, const T& items) {
	stringstream ss;
	ss << name << endl;
	ss << "{" << items.x << "," << items.y << "," << items.z << "}";

	cout << ss.str() << endl << endl;
}

//#define printm16(var) printm16_without_macro(#var, var)
//void printm16_without_macro(const string& name, const std::array<float, 16>& matrix) {
//	stringstream ss;
//	ss << name << endl;
//	//ss << "{";
//	for (int row = 0; row < 4; ++row) {
//		for (int col = 0; col < 4; ++col) {
//			// For column-major order, the column changes fastest.
//			ss << matrix[col * 4 + row] << " ";
//		}
//		ss << std::endl; // Newline at the end of each row
//	}
//	//ss << "}";
//	cout << ss.str() << endl << endl;
//}





//Que hace esto?
//aun con p chatgpt sigue diciendo que referencia podría no ser lo mejor. También sigue diciendo que un valor positivo es que p2 está a la derecha
template<typename T>
float crossProduct(const vec2<T>& p0, const vec2<T>& p1, const vec2<T>& p2) {
	vec2<T> v01 = { p1.x - p0.x, p1.y - p0.y };
	vec2<T> v12 = { p2.x - p1.x, p2.y - p1.y };

	return v01.x * v12.y - v01.y * v12.x;
}










//consts, cambiale el nombre y deja el nombre "is" para bools
float isRightOfLine(p& A, p& B, p& P);



std::array<float, 4> createQuaternion(float angle, const p3& axis);

std::array<float, 4> inverseQuaternion(const std::array<float, 4>& q);

std::array<float, 4> multiplyQuaternions(const std::array<float, 4>& a, const std::array<float, 4>& b);

// q * p * q^-1
p3 rotatePoint(const p3& point, const std::array<float, 4>& rotationQuaternion);

float isBelowTriangle(const p3& a, const p3& b, const p3& c, const p3& p);