//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 13/02/2024
//

#include "main.h"

DatabaseInterface* databaseConnection;
FileSystemInterface* fileManager;

//Helper method: password registration method for registerNewUser() and changePassword()
std::string fetchPassword(){
    std::string newPassword;
    std::string confirmPassword;

    for (int passwordAttempts = 0; passwordAttempts < 3; ++passwordAttempts) {
        std::cout << "Please set a new password> ";

        std::cin >> newPassword;
        std::cout << std::endl;

        std::cout << "Please confirm the new password> ";

        std::cin >> confirmPassword;
        std::cout << std::endl;

        //Breaks loop if password entry is confirmed
        if (newPassword == confirmPassword) {
            //returns password here
            return newPassword;

        } else {
            std::cout << "The Passwords must match. Please try again." << std::endl << std::endl;
        }
    }

    std::cout << "Password not registered after three failed set password attempts" << std::endl;
    exit(0);
}

//Takes the user through the password creation wizard
void registerNewUser() {

    std::cout << "Welcome to Key Manager\n" << std::endl;

    std::string password = fetchPassword();

    //Inserts into database here
    databaseConnection->insertUser(password);

}

//Verifies user password
void checkPassword(std::string password) {
    //first we fetch the username so that we can query using it
    std::string username = getlogin();

    //mow we do the actual query to fetch the password
    std::string storedPassword = databaseConnection->findUserPassword(username);

    //then we compare the passwords. Add logs depending on success
    if(storedPassword == password){
        databaseConnection->addLog("Successful login conducted by user: " + username);
    } else {
        databaseConnection->addLog("Failed login attempt made by user: " + username);
        std::cout << "Incorrect Password." << std::endl;
        exit(0);
    }

}

//Boilerplate guidance on how to use program
void printGuidance(){

    std::cout <<"--help provides a how-to-use" << std::endl
              <<"--chpass <password> Changes your password" << std::endl
              <<"--seeKeys <password> Displays all keys" << std::endl
              <<"--seeLogs <password> Displays all logs" << std::endl
              <<"--rmKey <password> <key name> Removes the specified key" << std::endl
              <<"--addKey <password> <key name> <path to file> Adds the key you provide" << std::endl
              <<"--updKey <password> <key name> <path to file> Replaces the existing key you have stored" << std::endl
              <<"<password> <key name> Places the key you request in the current directory." << std::endl
              <<"<password> <key name> <path to directory> Places the key in a specified directory." << std::endl;
}

//A password changing wizard.
void changePassword(std::string password){
    checkPassword(password);

    std::string newPassword = fetchPassword();

    databaseConnection->changeUserPassword(newPassword);
}

//Shows all stored keys in the database
void showKeys(std::string password){
    checkPassword(password);

    //Printf Format rule: String of 15 chars, followed by 4 Whitespaces,
    // followed by a string of any length, followed by a line break.
    std::string formatRule = "%-15s%-4s%s\n";

    //fetch keys from database
    std::vector<DatabaseInterface::KeyEntry> keyVector = databaseConnection->getAllKeys();

    //print the header
    printf(formatRule.c_str(), "Key Sample", "", "Key Name");

    //print the keys
    for(DatabaseInterface::KeyEntry entry : keyVector) {
           printf(formatRule.c_str(), entry.extendedKeySample.c_str(), "", entry.keyName.c_str());
    }

    //log the event
    databaseConnection->addLog("Keys viewed by user (ALL keys)");
}

//shows all logs in the database
void showLogs(std::string password){
    checkPassword(password);

    //Printf Format rule: String of 15 chars, followed by 4 Whitespaces,
    // followed by a string of any length, followed by a line break.
    std::string formatRule = "%-19s%-4s%s\n";

    std::vector<DatabaseInterface::LogEntry> logVector = databaseConnection->getAllLogs();

    //print the header
    printf(formatRule.c_str(), "Log Date/Time", "", "Log Description");

    //Print the keys
    for(DatabaseInterface::LogEntry entry : logVector) {
        printf(formatRule.c_str(), entry.logDate.c_str(), "", entry.logContent.c_str());
    }

    //log the event
    databaseConnection->addLog("Logs viewed by user (ALL logs)");
}

//Deletes the file after 15 minutes
void selfDestuctKey(const std::string& path){
    //pauses the self-destruction for 15 minutes
    std::this_thread::sleep_for(std::chrono::minutes(2));
    fileManager->deleteFile(path);
}

//Gives the user the key in the specified directory
void exportKey(std::string password, std::string keyName, std::string path){
    checkPassword(password);

    DatabaseInterface::Key file = databaseConnection->findKey(keyName);

    //Mappings to a new KeyFile struct
    bool fileSaved = fileManager->saveFile({file.keyType, file.keyContents}, path);

    if(fileSaved){
        databaseConnection->addLog("Key \"" + keyName + "\" was exported to a file.");
        std::cout << "File created. Will self-destruct in 15 mins." << std::endl;

        //create the path
        std::string fullDirectory = path + "/" + fileManager->getKeyFileName() + file.keyType;

        //Create a thread
        std::thread selfDestructThread(selfDestuctKey, fullDirectory);

        //Thread will now run in the background
        selfDestructThread.detach();

    } else {
        databaseConnection->addLog("Failed attempt to export key\"" + keyName + "\" to a file.");
    }
}

//Gives user the key in the current directory
void exportKey(std::string password, std::string keyName){
    exportKey(password, keyName, fileManager->getCurrentDirectory());
}

//deletes a stored key
void removeKey(std::string password, std::string keyName){
    checkPassword(password);

    databaseConnection->deleteKey(keyName);

    std::cout << keyName << " Key deleted successfully." << std::endl;
}

//Adds a new key to the database
void addNewKey(std::string password, std::string keyName, std::string path){
    checkPassword(password);

    //Fetch key file from system and log it
    FileSystemInterface::KeyFile keyFromFile = fileManager->getFileAsString(path);
    databaseConnection->addLog("New file (\"" + path + "\") read by client.");

    //Insert that file into the database -  Mappings to a new Key struct
    databaseConnection->insertKey(keyName, {keyFromFile.fileContents, keyFromFile.fileExtension});
}

//Replaces an existing key with the given key
void updateKey(std::string password, std::string keyName, std::string path){
    checkPassword(password);

    //fetch key from file
    FileSystemInterface::KeyFile key = fileManager->getFileAsString(path);

    //put key in database
    databaseConnection->updateKey(keyName, {key.fileContents, key.fileExtension});
}

//Determines and diverts the program based upon the arguments given on program call
void determineServiceRequest(int argc, char** argv){

    // Only one argument given aside from program call
    if(argc < 2){
        std::cout << "This command cannot be run on its own. For more information, run again using the"
                  << "For more information run with the --help flag\n\n";

    } else if(argc == 2 && std::string(argv[1])  == "--help"){
        // --help provides a how-to-use
        printGuidance();

    } else if(argc == 3 && std::string(argv[1]) == "--chpass"){
        //--chpass <password> requires a password and allows you to change your password
        changePassword(argv[2]);

    } else if(argc == 3 && std::string(argv[1]) == "--seeKeys"){
        //--seeKeys <password> requires a password and will display all keys
        showKeys(argv[2]);

    } else if(argc == 3 && std::string(argv[1]) == "--seeLogs"){
        //--seeLogs <password> requires a password and will display all Logs
        showLogs(argv[2]);

    } else if(argc == 3){
        //<password> <key name> places a key wherever the user is
        exportKey(argv[1], argv[2]);

    } else if(argc == 4 && std::string(argv[1]) == "--rmKey"){
        //--rmKey <password> <key name> removes the specified key
        removeKey(argv[2], argv[3]);

    } else if(argc == 4){
        //<password> <key name> <path to directory> places the key in a specified directory.
        exportKey(argv[1], argv[2], argv[3]);

    } else if(argc == 5 && std::string(argv[1]) == "--addKey"){
        //--addKey <password> <key name> <path to file> adds the specified key
        addNewKey(argv[2], argv[3], argv[4]);

    } else if(argc == 5 && std::string(argv[1]) == "--updKey"){
        //--updKey <password> <key name> <path to file> replaces an existing key
        updateKey(argv[2], argv[3], argv[4]);

    } else {
        //Invalid command
        std::cout << "KeyMan: That command could not be recognised. "
                  << "To see how to use this program, run again with the \"--help\" flag. " << std::endl;
    }

}

int main(int argc, char** argv) {

    databaseConnection = new DatabaseInterface();
    fileManager = new FileSystemInterface();

    if(!databaseConnection->userRegistered()){
        registerNewUser();

    } else {
        determineServiceRequest(argc, argv);
    }
    return 0;
}