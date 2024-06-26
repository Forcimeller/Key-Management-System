//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 29/03/24.
//

#include "FileSystemInterface.h"

/* Public: Constructor */
FileSystemInterface::FileSystemInterface(){
    this->currentDirectory = std::filesystem::current_path();
}

/* Public: Responsible for opening the calling the file reader
   and returning the file as a string */
FileSystemInterface::KeyFile FileSystemInterface::getFileAsString(std::string path) {

    KeyFile fileData = readFile(path);

    return fileData;
}

/* Public: Returns the Current directory */
std::string FileSystemInterface::getCurrentDirectory(){
    return this->currentDirectory;
}

/* Public: Responsible for creating files in the current directory */
bool FileSystemInterface::saveFile(FileSystemInterface::KeyFile file, std::string directory) {

    try {
        std::ofstream newFile(directory + "/" + keyFileName + file.fileExtension);
        newFile << file.fileContents << std::endl;
        newFile.close();

        return true;

    } catch(const std::exception& error) {
        std::cout << "File could not be created" << std::endl;
    }
    return false;
}

/* Public: Responsible for deleting the key file from the current directory */
bool FileSystemInterface::deleteFile(std::string filePath) {

    try {
        if(std::filesystem::remove(filePath)){  //File removal here
            return true;
        } else {
            return false;
        }
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "There was an error removing the file." << std::endl;
    }
    return false;
}

/* Public: Responsible for retunting the Keyfile name */
std::string FileSystemInterface::getKeyFileName() {
    return this->keyFileName;
}

/* Private: Responsible for reading files from the directory */
FileSystemInterface::KeyFile FileSystemInterface::readFile(std::string path) {
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
    KeyFile fileData = {pathObject.extension().string(), file};

    return fileData;
}