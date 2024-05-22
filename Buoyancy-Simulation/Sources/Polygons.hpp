#pragma once

#include "Common.hpp"

#include "Lines3d.hpp"



//esto va a tener toda la funcionalidad. Si no la quieres cambiale el nombre a physicsPolygon y crea otra con menos cosas
struct Polygons {
	GLenum usageHint = GL_DYNAMIC_DRAW;

	bool isBufferUpdated = false;

	Lines3d lines;

	//vector<p3> model;
	vector<p3>& positions = lines.positions;
	vector <unsigned int> indices; //triangle indices, do not mistake with the lines indices
	vector<p3> xyPositions;

	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;

	p3 normal;

	//Setting initial buffers size
	size_t currentPositionsBufferSize = 0;
	size_t currentIndicesBufferSize = 0;
	size_t currentPositionsDataSize = 0;
	size_t currentIndicesDataSize = 0;


	//saves positions with its indices
	struct StructuredPoints {
		p point;
		unsigned int index;

		StructuredPoints() : point({ 0,0 }), index(0) {} //initializing paramenters to 0 if not specified 

		StructuredPoints(p point_, unsigned int index_)
			: point(point_), index(index_) {}

	};

	//I'm letting these outside so I don't create references for the auxiliars of sweep
	std::vector<std::deque<unsigned int>> chain;
	std::vector<StructuredPoints> sPoints;




	Polygons() {
		genBuffers();
	}

	~Polygons();

	void genBuffers();

	void draw();


	void calculateNormal() {
		for (int i = 0; i < positions.size() - 1; i++)
		{
			normal = normalize3(cross3(positions[i + 1] - positions[i], positions[i + 2] - positions[i + 1]));
			if (normal != p3{ 0, 0, 0 }) return;
		}
	}



	void clear();

	void addSet(vector<p3> items);

	void sweepTriangulation(/*int i*/);

	//auxiliar functions for sweepTriangulation
	void trChainBack(const unsigned int& currentChain);
	void trChainFront(const unsigned int& currentChain);


	//void ConvexTriangulation();

	void translate(p3 pos) {
		for (auto& position : positions)
		{
			position += pos;
		}
		isBufferUpdated = true;
	}
	
};