#pragma once

#include "Common.hpp"





struct Polygons2D {
	GLenum usageHint = GL_DYNAMIC_DRAW;

	bool isBufferUpdated = false;

	//hardcoding values just for testing
	vector<p2> positions = { {0,0},{200,0},{200,200},{0,200},{0,0} };
	vector <unsigned int> indices = { 0,1,2,2,3,0 };


	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;



	//Setting initial buffers size
	size_t currentPositionsBufferSize = 0;
	size_t currentIndicesBufferSize = 0;
	size_t currentPositionsDataSize = 0;
	size_t currentIndicesDataSize = 0;

	Polygons2D() {
		genBuffers();
	}

	void genBuffers() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	void draw() {
		isBufferUpdated = true; //just for debugging

		glBindVertexArray(vertexArray);

		////flag activates only when an addSet is called, otherwise the buffer remains the same
		if (isBufferUpdated)
		{
			currentPositionsDataSize = positions.size() * sizeof(p2);
			currentIndicesDataSize = indices.size() * sizeof(unsigned int);

			//Estoy haciendo pruebas y he tenido que poner la nueva condición. No creo que sea necesaria en el futuro
			if (currentPositionsDataSize > currentPositionsBufferSize || currentIndicesDataSize > currentIndicesBufferSize)
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
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}


	~Polygons2D() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}


	



	void clear() {
		indices.clear();
		positions.clear();
	}

};