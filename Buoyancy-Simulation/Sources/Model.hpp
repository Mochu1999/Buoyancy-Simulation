#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include "Common.hpp"

//A lo mejor meter esto en binaries manager. En la versión 2d queda más bonito

void writeModel(const std::vector<p3>& model) {
	std::string basePath = "Resources/Plain models/";
	std::string modelPath;

	std::cout << "Current models: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) {
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::string path = basePath + modelPath;
	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile) {
		size_t size = model.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

		// Write each point in the model
		outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}



// Function to read a model from a binary file
std::vector<p3> readModel() {
	std::string basePath = "Resources/Plain models/";
	std::string modelPath;
	std::cout << "set model path to one of these: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) {
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::vector<p3> model;
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) {
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();

	return model;
}

std::vector<p3> readModel(const std::string modelPath) {
	std::string basePath = "Resources/Plain models/";

	std::vector<p3> model;
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) {
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();

	return model;
}

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
			//print(normal);
		}
		else if (word == "vertex") {
			// Read and store vertex position
			iss >> vertex.x >> vertex.y >> vertex.z;
			positions.push_back(vertex);
			//print(vertex);
		}
	}

	inFile.close();
}


// Function to write a polyhedra without dividing its surfaces in polygons
void writeSimplePolyhedra(const std::vector<p3>& model, const std::vector<p3>& normals, const std::vector<unsigned int>& indices) {
	std::string basePath = "Resources/Simple polyhedra/";
	std::string modelPath;

	// List available models in the directory
	std::cout << "Current models: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) 
	{
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::string path = basePath + modelPath;
	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile) 
	{
		size_t size = model.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(p3));

		size = normals.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(normals.data()), size * sizeof(p3));

		size = indices.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(indices.data()), size * sizeof(unsigned int));
	}
	else 
	{
		std::cerr << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}

void readSimplePolyhedra(std::vector<p3>& model, std::vector<p3>& normals, std::vector<unsigned int>& indices, const std::string& modelPath) {
	std::string basePath = "Resources/Simple polyhedra/";
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) 
	{
		size_t size;

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		normals.resize(size);
		inFile.read(reinterpret_cast<char*>(normals.data()), size * sizeof(p3));

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		indices.resize(size);
		inFile.read(reinterpret_cast<char*>(indices.data()), size * sizeof(unsigned int));
	}
	else 
	{
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();
}