//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 29/03/24.
//

#ifndef CMDLINE_FILESYSTEMINTERFACE_H
#define CMDLINE_FILESYSTEMINTERFACE_H

#include <iostream>
#include <fstream>
#include <filesystem>

class FileSystemInterface {
public:
    FileSystemInterface();
    std::string getFileAsString(std::string path);

private:
    std::filesystem::path currentDirectory;
    bool createFile(std::string fileContents, std::string path);
    std::string readFile(std::string path);
    bool deleteFile(std::string path);

};


#endif //CMDLINE_FILESYSTEMINTERFACE_H
