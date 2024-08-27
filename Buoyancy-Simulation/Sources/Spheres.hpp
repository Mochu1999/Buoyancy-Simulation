#pragma once

#include "Common.hpp"
#include "Delaunay_2D.hpp"
#include "Lines3d.hpp"

//Creates the points of a Fibonacci Sphere with n points and a defined radius
std::vector<p3> addFibSphere(int n, float radius) {
	std::vector<p3> points;
	points.reserve(n);

	//points are separated horizontally at an angle (the golden angle) from the previous point. 
	const float goldenAngle = PI * (3.0 - std::sqrt(5.0));
	float nInv = 1.0f / n;

	for (int i = 0; i < n; ++i) {
		float y = -1.0 + (2.0 * i + 1.0) * nInv; //Points are distributed vertically from -1 to 1, evenly spaced

		// Calculate radius at this height to the vertical axis to ensure points lie on the sphere surface
		float r = std::sqrt(1.0 - y * y);

		float theta = goldenAngle * i;

		float x = r * std::cos(theta);
		float z = r * std::sin(theta);

		points.emplace_back(p3{ x * radius, y * radius, z * radius });
	}

	//printv3(points);
	return points;
}

std::vector<p> stereographicProjection(const std::vector<p3>& positions) {
	std::vector<p> projectedPoints;
	projectedPoints.reserve(positions.size());

	for (const auto& point : positions) {
		// Normalize the point to the unit sphere
		float distance = std::sqrt(point.x * point.x + point.y * point.y + point.z * point.z);
		float nx = point.x / distance;
		float ny = point.y / distance;
		float nz = point.z / distance;


		float projectionScale = 2.0 / (1.0 + ny);  // Standard stereographic projection formula

		p projectedPoint;
		projectedPoint.x = projectionScale * nx;

		projectedPoint.y = projectionScale * nz;

		projectedPoints.emplace_back(projectedPoint);
	}

	return projectedPoints;
}

//Hola, esto esta turbo caotico, vas a tener que darte una buena vuelta, lo siento. isBufferUpdated funciona mal. mesh no está del todo aislado, creo
// que me hubiera gustado separar completamente la funcionalidad. Si lo haces comenta que addSet tiene que estar siempre antes de los indices y decide 
// si te convence que el metodo sea así, a mi ahora mismo es que no podría importarme menos. También acabo de darme cuenta que no estoy tomando las 
// posiciones, lo haría en una funcion aparte, classic addSet. También haría que n_ de normal se haga un valor que sea función del radio si no se 
// especifica. Ya lo siguiente va a ser full iluminación. Siento dejarte con el marrón, siento no haber sido suficiente, siento haber malgastado mi día
// , por algún motivo tengo confianza en ti, siento el patronizing, no tengo ningún derecho, pero no quiero morir sin haber conseguido nada. Tú te irás
// a la cama miserable como me voy yo, solo podemos creer en que si nos sacrificamos el suficiente tiempo ,alguién se irá a la cama satisfecho de una vez.

//If you want to draw the mesh lines instead of the triangles, it is currently using Lines3d for it
struct Sphere {
	float radius;
	unsigned int n;

	Lines3d meshLines;

	vector<p3> positions;
	vector<p3> normals;

	vector<unsigned int> indices;
	vector<unsigned int> meshIndices;

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

	Sphere(float radius_, unsigned int n_ = std::numeric_limits<unsigned int>::max()) : radius(radius_) {
		genBuffers();

		if (n_ != std::numeric_limits<unsigned int>::max())
			n = n_;
		else
		{
			n = unsigned int(pow(radius, 2.5));
			n = std::clamp(n, 400u, 10000u);
		}
		

		positions = addFibSphere(n, radius);



		std::vector<p> projectedPoints = stereographicProjection(positions);


		vector<unsigned int> lidIndices;
		vector<Triangle> delaunay = bowyerWatson(projectedPoints, lidIndices);



		vector<p> lidPoints;
		for (const auto& i : lidIndices)
		{
			//already projected
			lidPoints.push_back(p{ positions[i].x,positions[i].z });

		}
		vector<Triangle> delaunay2 = bowyerWatson(lidPoints);


		meshLines.addSet(positions);


		meshIndices = generateMeshIndices(projectedPoints, delaunay);
		vector<unsigned int> secondMeshIndices = generateMeshIndices(lidPoints, delaunay2);
		indices.reserve(indices.size() + secondMeshIndices.size());
		meshIndices.insert(meshIndices.end(), secondMeshIndices.begin(), secondMeshIndices.end());
		meshLines.indices = meshIndices;

		

		indices = generateTrIndices(projectedPoints, delaunay);
		vector<unsigned int> secondTrIndices = generateTrIndices(lidPoints, delaunay2);
		indices.reserve(indices.size() + secondTrIndices.size());
		indices.insert(indices.end(), secondTrIndices.begin(), secondTrIndices.end());
	}

	void addSet(p3 location) {
		for (auto& pos : positions)
		{
			pos += location;
		}
		isBufferUpdated = true;

		meshLines.addSet(positions);
		meshLines.indices = meshIndices;
	}

	//las tapas van mal
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
	void drawLines() {
		meshLines.draw();
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

	~Sphere() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);

		positions.clear(); indices.clear();
	}
};