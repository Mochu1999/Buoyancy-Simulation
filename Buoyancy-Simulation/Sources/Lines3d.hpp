#pragma once

#include "Common.hpp"




struct Lines3d {

	GLenum usageHint = GL_DYNAMIC_DRAW; //Pass this into the constructor for the final version

	bool isBufferUpdated = false;

	bool cadMode = false;
	bool isHovering = false;

	vector<p3> positions;

	//indices must be given in a flat array because opengl expects them as such
	vector <unsigned int> indices;

	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;

	//Setting initial buffers size
	size_t currentPositionsBufferSize = 0;
	size_t currentIndicesBufferSize = 0;
	size_t currentPositionsDataSize = 0;
	size_t currentIndicesDataSize = 0;


	Lines3d() {
		genBuffers();
	}



	void genBuffers() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}



	void draw() {

		glBindVertexArray(vertexArray);

		//flag activates only when an addSet is Called, otherwise the buffer remains the same
		if (isBufferUpdated)
		{
			currentPositionsDataSize = positions.size() * sizeof(p3);
			currentIndicesDataSize = indices.size() * sizeof(unsigned int);

			//buffers are not equal
			if (currentPositionsDataSize > currentPositionsBufferSize)
			{
				currentPositionsBufferSize = currentPositionsDataSize * 2;
				currentIndicesBufferSize = currentIndicesDataSize * 2;

				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, currentPositionsBufferSize, nullptr, usageHint);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentIndicesBufferSize, nullptr, usageHint);
			}

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, currentPositionsDataSize, positions.data());


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, currentIndicesDataSize, indices.data());

			isBufferUpdated = false;
		}
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);

	}


	~Lines3d() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);

		positions.clear(); indices.clear();
	}


	//One set per lines
	void addSet(const vector<p3>& items, int isConsecutiveIndices = 1) {

		positions.clear(); 
		positions.reserve(items.size());
		positions.insert(positions.end(), items.begin(), items.end());

		//consecutive
		if (isConsecutiveIndices==1)
		{
			indices.clear();
			indices.reserve(items.size());
			for (unsigned int i = 0; i < items.size() - 1; i++)
			{
				indices.emplace_back(i);
				indices.emplace_back(i + 1);
			}
		}
		//pairs
		else if (isConsecutiveIndices == 2)
		{
			indices.clear();
			indices.reserve(items.size());
			for (unsigned int i = 0; i < items.size() - 1; i+=2)
			{
				indices.emplace_back(i);
				indices.emplace_back(i + 1);
			}
		}
		isBufferUpdated = true;
	}



};


