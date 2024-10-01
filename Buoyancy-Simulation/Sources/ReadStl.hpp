#pragma once

#include "Common.hpp"

// Function to read ASCII STL file

// Function to read ASCII STL file similar to your custom style
void readSTL(std::vector<p3>& positions, std::vector<p3>& normals, const std::string& filepath) {
	std::string basePath = "Resources/stl models/";
	std::ifstream inFile(basePath + filepath);

	

	if (!inFile) {
		std::cerr << "Unable to open file: " << filepath << std::endl;
		return;
	}

	std::string line;
	p3 normal, vertex;

	while (std::getline(inFile, line)) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;

		if (word == "facet") {
			// Read normal vector
			iss >> word; // Skip "normal"
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
			print(normal);
		}
		else if (word == "vertex") {
			// Read and store vertex position
			iss >> vertex.x >> vertex.y >> vertex.z;
			positions.push_back(vertex);
			print(vertex);
		}
	}

	inFile.close();
}