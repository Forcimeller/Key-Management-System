//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 29/03/24.
//

#include "FileSystemInterface.h"

std::string FileSystemInterface::getFileAsString(std::string path) {

    std::string fileContents = readFile(path);

    return fileContents;
}

bool FileSystemInterface::saveFile(std::string fileContents) {

    std::ofstream newFile("keyfile");

    newFile << fileContents << std::endl;

    newFile.close();

    return true;
}

std::string FileSystemInterface::readFile(std::string path) {
    std::string file = "";

    //convert string path into path object
    std::filesystem::path pathObject(path);

    //Open the file creating a stream
    std::ifstream sourceFile(pathObject);

    //Verify vile opening successful
    if(sourceFile.is_open()){
        std::string lineInFile;

        //Loop through each line in file
        while(std::getline(sourceFile, lineInFile)){
            file.append(lineInFile).append("\n");
        }
    } else {
        std::cout << "The specified file could not be found" << std::endl;
        exit(0);
    }

    return file;
}

bool FileSystemInterface::deleteFile(std::string path) {
    return false;
}

 FileSystemInterface::FileSystemInterface(){
    this->currentDirectory = std::filesystem::current_path();
}
