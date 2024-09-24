#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include "Common.hpp"
#include "Camera.hpp"


struct Settings {
    std::string settingsPath = "Resources/Settings.bin";

    p3& cameraPos;
    p3& forward;

    enum Variables {
        CameraPos,
        Forward
    };

    Settings(Camera& camera): cameraPos(camera.cameraPos), forward(camera.forward) {
        read();      
    }

    //It first writes the enum value of the variable, and then the variable
    void write() {
        std::ofstream outFile(settingsPath, std::ios::binary);
        if (outFile) {
            
            Variables var = CameraPos;
            outFile.write(reinterpret_cast<const char*>(&var), sizeof(var));
            outFile.write(reinterpret_cast<const char*>(&cameraPos), sizeof(cameraPos));

            
            var = Forward;
            outFile.write(reinterpret_cast<const char*>(&var), sizeof(var));
            outFile.write(reinterpret_cast<const char*>(&forward), sizeof(forward));
        }
        outFile.close();
    }

    //Each variable is stored in two parts. First the enum variable is read, and then it will enter a switch case to read its associate
    void read() {
        std::ifstream inFile(settingsPath, std::ios::binary);

        if (inFile) {
            while (inFile) {
                Variables var;
                if (!inFile.read(reinterpret_cast<char*>(&var), sizeof(var))) break;

                switch (var) {
                case CameraPos:
                    inFile.read(reinterpret_cast<char*>(&cameraPos), sizeof(cameraPos));
                    break;
                case Forward:
                    inFile.read(reinterpret_cast<char*>(&forward), sizeof(forward));
                    break;
                default:
                    std::cerr << "Unknown variable in settings file." << std::endl;
                    break;
                }
            }
        }
        else {
            std::cout << "Settings not found? This should not happen" << std::endl;
        }
        inFile.close();

        // Display the results
        /*print(cameraPos);
        print(forward);*/
    }
};


