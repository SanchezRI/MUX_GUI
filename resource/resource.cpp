#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <direct.h>
#include "resource.h"

std::string iconRelPath = "include/icons/lhep.ico";
std::string resourcesFilePath = "resource/resources.rc";

static int ChangeIconPath() {
    char buffer[FILENAME_MAX];
    _getcwd(buffer, FILENAME_MAX);
    std::string currentPath(buffer);
    std::cout << "Current working directory: " << currentPath << std::endl;

    std::string prefix = "MUX_GUI";
    size_t pos = currentPath.find(prefix);

    if (pos != std::string::npos) {
        std::string basePath = currentPath.substr(0, pos);
        std::string iconPath = basePath + iconRelPath;

        std::ifstream resourcesFile(resourcesFilePath);
        std::string line;
        std::string updatedContent;

        if (resourcesFile.is_open()) {
            while (getline(resourcesFile, line)) {
                if (line.find("IDI_ICON1 ICON") != std::string::npos) {
                    line = "IDI_ICON1 ICON \"" + iconPath + "\"";
                }
                updatedContent += line + "\n";
            }
            resourcesFile.close();
        }
        else {
            std::cerr << "Cannot open resources file: " << resourcesFilePath << std::endl;
            return 1;
        }

        std::ofstream outputFile(resourcesFilePath);
        if (outputFile.is_open()) {
            outputFile << updatedContent;
            outputFile.close();
            std::cout << "Updated resources.rc with new icon path: " << iconPath << std::endl;
        }
        else {
            std::cerr << "Cannot open resources file for writing: " << resourcesFilePath << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "'MUX_GUI' not found in current path." << std::endl;
    }
    return 0;
}
