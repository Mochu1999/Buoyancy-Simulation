#pragma once
#include"Common.hpp"



struct Pyramid {


	vector<p3> positions = {
		// COORDINATES as p3 structs
		p3(-0.5f, 0.0f,  0.5f),  // Bottom side
		p3(-0.5f, 0.0f, -0.5f),  // Bottom side
		p3(0.5f, 0.0f, -0.5f),  // Bottom side
		p3(0.5f, 0.0f,  0.5f),  // Bottom side

		p3(-0.5f, 0.0f,  0.5f),  // Left Side
		p3(-0.5f, 0.0f, -0.5f),  // Left Side
		p3(0.0f, 0.8f,  0.0f),  // Left Side

		p3(-0.5f, 0.0f, -0.5f),  // Non-facing side
		p3(0.5f, 0.0f, -0.5f),  // Non-facing side
		p3(0.0f, 0.8f,  0.0f),  // Non-facing side

		p3(0.5f, 0.0f, -0.5f),  // Right side
		p3(0.5f, 0.0f,  0.5f),  // Right side
		p3(0.0f, 0.8f,  0.0f),  // Right side

		p3(0.5f, 0.0f,  0.5f),  // Facing side
		p3(-0.5f, 0.0f,  0.5f),  // Facing side
		p3(0.0f, 0.8f,  0.0f)   // Facing side
	};


	vector<unsigned int> indices =
	{
		0, 1, 2, // Bottom side
		0, 2, 3, // Bottom side
		4, 6, 5, // Left side
		7, 9, 8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
	};

	vector<float> intNormals = {
		// NORMALS
		0.0f, -1.0f, 0.0f,  // Bottom side
		0.0f, -1.0f, 0.0f,  // Bottom side
		0.0f, -1.0f, 0.0f,  // Bottom side
		0.0f, -1.0f, 0.0f,  // Bottom side

		-0.8f, 0.5f,  0.0f,  // Left Side
		-0.8f, 0.5f,  0.0f,  // Left Side
		-0.8f, 0.5f,  0.0f,  // Left Side

		0.0f, 0.5f, -0.8f,  // Non-facing side
		0.0f, 0.5f, -0.8f,  // Non-facing side
		0.0f, 0.5f, -0.8f,  // Non-facing side

		0.8f, 0.5f,  0.0f,  // Right side
		0.8f, 0.5f,  0.0f,  // Right side
		0.8f, 0.5f,  0.0f,  // Right side

		0.0f, 0.5f,  0.8f,  // Facing side
		0.0f, 0.5f,  0.8f,  // Facing side
		0.0f, 0.5f,  0.8f   // Facing side
	};
	vector<p3> normals;



	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;

	void calculateNormals() {
		normals.clear();
		normals.resize(positions.size(), { 0,0,0 });

		for (int i = 0; i < indices.size(); i += 3) {
			vec3<float> pos1 = positions[indices[i]];
			vec3<float> pos2 = positions[indices[i + 1]];
			vec3<float> pos3 = positions[indices[i + 2]];

			vec3<float> normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));

			normals[indices[i]] = normals[indices[i]] + normal;
			normals[indices[i + 1]] = normals[indices[i + 1]] + normal;
			normals[indices[i + 2]] = normals[indices[i + 2]] + normal;
		}

		// Normalize the normals
		for (vec3<float>& normal : normals) {
			normal = normalize3(normal);
		}
	}


	Pyramid() {
		for (auto& p : positions)
		{
			//printp3(p);
			p *= 100;
			p += p3{ 100,0,100 };
		}
		printv3(positions);
		calculateNormals();
		genBuffers();
	}



	void genBuffers() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// Generate and bind position buffer
		GLuint positionBuffer;
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(p3), positions.data(), GL_STATIC_DRAW);

		// Set up position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Generate and bind normal buffer
		GLuint normalBuffer;
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(p3), normals.data(), GL_STATIC_DRAW);

		// Set up normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		// Generate and bind index buffer (unchanged)
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}





	void draw() {

		glBindVertexArray(vertexArray);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}


	~Pyramid() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}
};