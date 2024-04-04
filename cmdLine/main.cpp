//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 13/02/2024
//

#include "main.h"

DatabaseInterface* databaseConnection;

std::string fetchPassword(){
    std::string newPassword;
    std::string confirmPassword;

    bool passwordsMatch = false;

    for (int passwordAttempts = 0; passwordAttempts < 3; ++passwordAttempts) {
        std::cout << "Please set a new password> ";

        std::cin >> newPassword;
        std::cout << std::endl;

        std::cout << "Please confirm the new password> ";

        std::cin >> confirmPassword;
        std::cout << std::endl;

        //Breaks loop if password entry is confirmed
        if (newPassword == confirmPassword) {
            passwordsMatch = true;
            //returns password here
            return newPassword;

        } else {
            std::cout << "The Passwords must match. Please try again." << std::endl << std::endl;
        }
    }

    std::cout << "Password not registered after three failed set password attempts" << std::endl;
    exit(0);
}

void registerNewUser() {

    std::cout << "Welcome to Key Manager\n" << std::endl;

    std::string password = fetchPassword();

    //Inserts into database here
    databaseConnection->insertUser(password);

}

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

void changePassword(std::string password){
    checkPassword(password);

    std::string newPassword = fetchPassword();

    databaseConnection->changeUserPassword(newPassword);
}

void showKeys(std::string password){
    checkPassword(password);

}

void showLogs(std::string password){
    checkPassword(password);

}

void exportKey(std::string password, std::string keyName, std::string path){
    checkPassword(password);

}

void removeKey(std::string password, std::string keyName){
    checkPassword(password);

}

void addNewKey(std::string password, std::string keyName, std::string path){
    checkPassword(password);

}

void updateKey(std::string password, std::string keyName, std::string path){
    checkPassword(password);

}


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
        std::string currentDirectory;
        exportKey(argv[1], argv[2], currentDirectory);

    } else if(argc == 4 && std::string(argv[1]) == "--rmKey"){
        //--rmKey <password> <key name> removes the specified key
        removeKey(argv[3], argv[4]);

    } else if(argc == 4){
        //<password> <key name> <path to directory> places the key in a specified directory.
        exportKey(argv[2], argv[3], argv[4]);

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

    if(!databaseConnection->userRegistered()){
        registerNewUser();

    } else {

        determineServiceRequest(argc, argv);

        //std::string password = "p";
        //checkPassword(password);
        //std::cout << argc << "  \n" << argv[1];
    }

    /*
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};

    auto database = client["KeyManDatabase"];
    auto collection = database["test"];

    auto find_one_filtered_result = collection.find_one(make_document(kvp("i", 0)));

    if (find_one_filtered_result) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No." << std::endl;
    }

    auto doc_value = make_document(
            kvp("name", "MongoDB"),
            kvp("type", "database"),
            kvp("count", 1)
    );

    //auto doc_view = doc_value.view();

    collection.insert_one(
            static_cast<bsoncxx::v_noabi::document::view_or_value>(doc_value)
            );

    find_one_filtered_result = collection.find_one(make_document(kvp("name", "MongoDB")));

    if (find_one_filtered_result) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No." << std::endl;
    }

     */
    return 0;
}







