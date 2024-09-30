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