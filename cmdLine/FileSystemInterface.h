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
    bool saveFile(std::string fileContents);
    bool deleteFile();

private:
    std::filesystem::path currentDirectory;
    const std::string keyFileName = "keyfile";
    std::string readFile(std::string path);


};


#endif //CMDLINE_FILESYSTEMINTERFACE_H
