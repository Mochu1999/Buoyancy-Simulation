#pragma once
#include"Common.hpp"



struct Pyramid {


	vector<p3> positions = { {50,0,150},{50,0,50},{150,0,50},{150,0,150}
	,{50,0,150},{50,0,50},{100,80,100},{50,0,50},{150,0,50},{100,80,100}
	,{150,0,50},{150,0,150},{100,80,100},{150,0,150},{50,0,150},{100,80,100} };

	vector<unsigned int> indices =
	{
		0, 1, 2, // Bottom side
		0, 2, 3, // Bottom side
		4, 6, 5, // Left side
		7, 9, 8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
	};
	
	void reorder() {
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

	void calculateNormals1() {
		normals.clear();
		normals.resize(positions.size(), { 0,0,0 });

		for (int i = 0; i < indices.size(); i += 3) {
			p3 pos1 = positions[indices[i]];
			p3 pos2 = positions[indices[i + 1]];
			p3 pos3 = positions[indices[i + 2]];

			p3 normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));

			normals[indices[i]] = normal;
			normals[indices[i + 1]] =normal;
			normals[indices[i + 2]] = normal;
		}

		// Normalize the normals
		for (vec3<float>& normal : normals) {
			normal = normalize3(normal);
		}
	}

	//Averaged normals. 
	//stores normals for each triangle face, then summing them for each vertex
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


	Pyramid() {
		reorder();
		calculateNormals2();
		
		//calculateNormals1();

		print(positions.size());
		print(indices.size()/3);
		print(normals.size());
		print(positions);
		print(indices);
		print(normals);
		genBuffers();
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
};