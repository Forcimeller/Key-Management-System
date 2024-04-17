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

    try {
        std::ofstream newFile(keyFileName);

        newFile << fileContents << std::endl;

        newFile.close();

        return true;

    } catch(const std::exception& error) {

        std::cout << "file could not be created" << std::endl;

    }
    return false;
}

std::string FileSystemInterface::readFile(std::string path) {
    std::string file;

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

bool FileSystemInterface::deleteFile() {

    try {
        if(std::filesystem::remove(keyFileName)){
            return true;
        } else {
            return false;
        }
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "There was an error removing the file." << std::endl;
    }
    return false;
}

 FileSystemInterface::FileSystemInterface(){
    this->currentDirectory = std::filesystem::current_path();
}
