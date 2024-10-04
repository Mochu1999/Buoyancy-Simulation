#pragma once
#include"Common.hpp"


//This file is here to test illumination. Here you will find sharp illumination, where you need to repeat positions if you want to have different normals per vertex.
//If you don't repeat positions you can only have one normal per vertex (soft illumination)
struct Pyramid {

	//vertex data is unique for each face (repeated positions) 
	vector<p3> positions = { {50,0,150},{50,0,50},{150,0,50},{50,0,150},{150,0,50},{150,0,150}
		,{50,0,150},{100,80,100},{50,0,50},{50,0,50},{100,80,100},{150,0,50},{150,0,50}
	,{100,80,100},{150,0,150},{150,0,150},{100,80,100},{50,0,150} };

	vector<unsigned int> indices;

	void reorder() {
		//why the hell didn't I use sets here
		vector<p3> interm;
		bool isInside;
		for (auto& i : positions)
		{
			isInside = false;
			for (auto& j : interm)
			{
				if (i == j)
				{
					isInside = true;
					break;
				}
			}
			if (!isInside)
				interm.push_back(i);
		}

		vector<unsigned int> intermi;

		for (auto& i : indices)
		{
			for (int j = 0; j < interm.size(); j++)
			{
				if (positions[i] == interm[j])
				{
					intermi.push_back(j);
					break;
				}
			}
		}


		positions.clear(); indices.clear();
		positions = interm; indices = intermi;
	}

	vector<p3> normals;
	bool isBufferUpdated = true;


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

	GLenum usageHint = GL_STATIC_DRAW;

	//vertex data must be unique for each face (repeated positions)
	void calculateNormals1() {
		normals.clear();
		normals.resize(positions.size(), { 0,0,0 });

		for (int i = 0; i < indices.size(); i += 3) {
			vec3<float> pos1 = positions[indices[i]];
			vec3<float> pos2 = positions[indices[i + 1]];
			vec3<float> pos3 = positions[indices[i + 2]];

			vec3<float> normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));

			normals[indices[i]] = normal;
			normals[indices[i + 1]] = normal;
			normals[indices[i + 2]] = normal;
		}

		// Normalize the normals
		for (vec3<float>& normal : normals) {
			normal = normalize3(normal);
		}
	}

	//normal averaging for shared vertices (unrepeated positions)
	void calculateNormals2() {
		normals.clear();
		normals.resize(positions.size(), { 0, 0, 0 });

		// First, compute per-triangle normals and sum them to the corresponding vertex normals
		for (int i = 0; i < indices.size(); i += 3) {
			p3 pos1 = positions[indices[i]];
			p3 pos2 = positions[indices[i + 1]];
			p3 pos3 = positions[indices[i + 2]];

			p3 normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));

			normals[indices[i]] += normal;
			normals[indices[i + 1]] += normal;
			normals[indices[i + 2]] += normal;
		}

		// Normalize all vertex normals
		for (p3& normal : normals) {
			normal = normalize3(normal);
		}
	}
	
	void generateIndices() {
		indices.clear();
		for (int i = 0; i < positions.size(); i++)
		{
			indices.push_back(i);
		}
	}

	Pyramid() {
		for (auto& pos : positions)
		{
			pos += {0, 0, 60};
		}
		//rotate3D(positions, 0, 0, 0);

		genBuffers();
		generateIndices();

		//reorder();
		//calculateNormals2();

		calculateNormals1();

		/*print(positions.size());
		print(indices.size() / 3);
		print(normals.size());
		print(positions);
		print(indices);
		print(normals);*/
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
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}





	void draw() {

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


	~Pyramid() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}


	//not tested, to have sharp edges
	void calculateNormalsWithSharpEdges(float sharpAngleDegrees) {
		normals.clear();
		normals.resize(positions.size(), { 0, 0, 0 });

		// Pre-calculate all face normals
		std::vector<vec3<float>> faceNormals(indices.size() / 3);

		for (int i = 0; i < indices.size(); i += 3) {
			vec3<float> pos1 = positions[indices[i]];
			vec3<float> pos2 = positions[indices[i + 1]];
			vec3<float> pos3 = positions[indices[i + 2]];

			vec3<float> faceNormal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
			faceNormals[i / 3] = faceNormal;
		}

		// Accumulate normals for each vertex but only if faces are smooth enough
		float cosSharpAngleThreshold = cos(sharpAngleDegrees * PI / 180.0f); // Convert degrees to radians

		for (int i = 0; i < indices.size(); i += 3) {
			int idx1 = indices[i];
			int idx2 = indices[i + 1];
			int idx3 = indices[i + 2];

			vec3<float> faceNormal = faceNormals[i / 3];

			// For each edge, compare the normals of adjacent faces
			addNormalIfSmooth(idx1, idx2, faceNormal, faceNormals, cosSharpAngleThreshold);
			addNormalIfSmooth(idx2, idx3, faceNormal, faceNormals, cosSharpAngleThreshold);
			addNormalIfSmooth(idx3, idx1, faceNormal, faceNormals, cosSharpAngleThreshold);
		}

		// Normalize the accumulated normals
		for (vec3<float>& normal : normals) {
			normal = normalize3(normal);
		}
	}

	// Helper function to accumulate normal for a vertex if the adjacent face normals are smooth
	void addNormalIfSmooth(int vertexIndex1, int vertexIndex2, vec3<float>& faceNormal,
		const std::vector<vec3<float>>& faceNormals, float cosThreshold) {
		for (int i = 0; i < indices.size(); i += 3) {
			// Check if vertexIndex1 and vertexIndex2 belong to another face
			if ((indices[i] == vertexIndex1 && indices[i + 1] == vertexIndex2) ||
				(indices[i] == vertexIndex2 && indices[i + 1] == vertexIndex1) ||
				(indices[i] == vertexIndex1 && indices[i + 2] == vertexIndex2) ||
				(indices[i] == vertexIndex2 && indices[i + 2] == vertexIndex1)) {

				vec3<float> adjacentNormal = faceNormals[i / 3];
				if (dot3(faceNormal, adjacentNormal) >= cosThreshold) {
					normals[vertexIndex1] += faceNormal;
					normals[vertexIndex2] += faceNormal;
				}
			}
		}
	}
};