#pragma once

#include "Common.hpp"


struct Fourier {
	vector<p3> positions;
	vector<p3> normals;

	int n = 2; //number of points in one axis of the square grid
	int segments = n - 1;
	float endPosition = 100;
	float startPosition = 0;
	float interval = (endPosition - startPosition) / segments;




	vector<Lines3d> lines;


	vector<unsigned int> indices;


	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;
	unsigned int normalsBuffer;




	//Setting initial buffers size
	size_t currentPositionsBufferSize = 0;
	size_t currentIndicesBufferSize = 0;
	size_t currentNormalsBufferSize = 0;

	size_t currentPositionsDataSize = 0;
	size_t currentIndicesDataSize = 0;
	size_t currentNormalsDataSize = 0;

	bool isBufferUpdated = false;
	GLenum usageHint = GL_DYNAMIC_DRAW;

	float period = 3000;
	float frecuency = 2 * PI / period;
	float amplitude = 10;
	float offset = 5;
	float phase = PI / 2;
	float phaseSpeed = 0.01;


	Fourier() {
		genBuffers();
		createWavePositions();
		//calculateNormals();
		createIndices();
		/*cout << "fourier: ";
		printv3(positions);
		printflat(indices);*/
		print(positions);
		print(normals);
		print(indices);
		print(positions.size());
		print(normals.size());
		print(indices.size());
	}

	float waveFunction(float x, float z) {
		return amplitude * sin(100 * x + phase) * sin(100 * z) + offset;
		//return amplitude * sin(100 * x + phase) + offset;
	}

	//this should only executed at the start
	void createWavePositions() {
		positions.clear();
		positions.reserve(n * n);

		for (float z = startPosition; z <= endPosition; z += interval)
		{
			for (float x = startPosition; x <= endPosition; x += interval)
			{
				positions.emplace_back(p3{ x,waveFunction(x,z),z });

				normals.emplace_back(calculateNormalFD(x, z));
			}
		}
		phase += phaseSpeed;
		isBufferUpdated = true;


		//lines
		lines.clear();
		lines.resize(indices.size() / 3);
		for (int i = 0; i < indices.size(); i += 3)
		{

			lines[i / 3].addSet({ positions[indices[i]],positions[indices[i + 1]], positions[indices[i + 2]] });
		}
	}

	//executed in the while loop
	void updateWavePositions() {
		size_t index = 0;
		for (float z = startPosition; z < endPosition; z += interval) {
			for (float x = startPosition; x < endPosition; x += interval) {
				positions[index].y = amplitude * sin(100 * x + phase) * sin(100 * z) + offset;
				index++;
			}
		}
		phase += phaseSpeed;
		isBufferUpdated = true;


	}

	//void calculateNormals() {
	//	normals.clear();
	//	normals.resize(positions.size(), { 0,0,0 });

	//	for (int i = 0; i < indices.size(); i += 3) {
	//		vec3<float> pos1 = positions[indices[i]];
	//		vec3<float> pos2 = positions[indices[i + 1]];
	//		vec3<float> pos3 = positions[indices[i + 2]];

	//		vec3<float> normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));

	//		normals[indices[i]] = normals[indices[i]]+ normal;
	//		normals[indices[i + 1]] = normals[indices[i + 1]]+ normal;
	//		normals[indices[i + 2]] = normals[indices[i + 2]]+ normal;
	//	}

	//	// Normalize the normals
	//	for (vec3<float>& normal : normals) {
	//		normal = normalize3(normal);
	//	}
	//}
	// Step size for finite difference (small value)
	const float epsilon = 0.01f;

	// Estimate the normal using finite differences
	p3 calculateNormalFD(float x, float z) {
		// Sample points nearby
		float heightL = waveFunction(x - epsilon, z); // Left
		float heightR = waveFunction(x + epsilon, z); // Right
		float heightD = waveFunction(x, z - epsilon); // Down
		float heightU = waveFunction(x, z + epsilon); // Up

		// Compute the tangent vectors
		p3 tangentX = { 2 * epsilon, heightR - heightL, 0 }; // X tangent (Left to Right)
		p3 tangentZ = { 0, heightU - heightD, 2 * epsilon }; // Z tangent (Down to Up)

		// Normal is the cross product of the two tangents
		p3 normal = cross3(tangentX, tangentZ); // Note: Ensure the right-hand rule is followed

		// Normalize the normal vector
		return normalize3(normal);
	}


	void createIndices() {
		indices.clear();

		indices.reserve(segments * segments * 6); //checked
		//indices.insert(indices.end(), { 0,3,1 });
		//print(segments);
		for (int z = 0; z < segments; ++z)
		{
			for (int x = 0; x < segments; ++x)
			{
				unsigned int topLeft = (z * n) + x;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = topLeft + n;
				unsigned int bottomRight = bottomLeft + 1;

				indices.insert(indices.end(), { topLeft,bottomLeft,topRight,topRight,bottomLeft,bottomRight });
			}
		}
	}

	void draw() {

		//calculateNormals();

		glBindVertexArray(vertexArray);

		if (isBufferUpdated)
		{
			currentPositionsDataSize = positions.size() * sizeof(p3);
			currentIndicesDataSize = indices.size() * sizeof(unsigned int);

			if (currentPositionsDataSize > currentPositionsBufferSize || currentIndicesDataSize > currentIndicesBufferSize)
			{
				currentPositionsBufferSize = currentPositionsDataSize * 2;
				currentIndicesBufferSize = currentIndicesDataSize * 2;

				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, currentPositionsBufferSize, nullptr, usageHint);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentIndicesBufferSize, nullptr, usageHint);

				glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
				glBufferData(GL_ARRAY_BUFFER, currentPositionsBufferSize, nullptr, usageHint);

			}

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, currentPositionsDataSize, positions.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, currentIndicesDataSize, indices.data());

			glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, currentPositionsDataSize, normals.data());

			isBufferUpdated = false;
		}



		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}


	void genBuffers() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
		glGenBuffers(1, &normalsBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalsBuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
	}

	~Fourier() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &normalsBuffer);
	}
};