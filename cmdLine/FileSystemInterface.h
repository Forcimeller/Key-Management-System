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
    //Public fields & Method Declarations

    //Needed for file type compatibility
    struct KeyFile {
        std::string fileExtension;
        std::string fileContents;
    };
    FileSystemInterface();
    KeyFile getFileAsString(std::string path);
    bool saveFile(FileSystemInterface::KeyFile file, std::string directory);
    bool deleteFile();
    std::string getCurrentDirectory();

private:
    //Private fields & Method Declarations
    std::filesystem::path currentDirectory;
    const std::string keyFileName = "keyfile";
    KeyFile readFile(std::string path);
};


#endif //CMDLINE_FILESYSTEMINTERFACE_H
