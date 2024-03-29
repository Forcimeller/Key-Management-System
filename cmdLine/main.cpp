//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 13/02/2024
//

#include "main.h"

DatabaseInterface* databaseConnection;

void addUserToDatabase(std::string password){

    std::string username = getlogin();

    bsoncxx::v_noabi::document::view_or_value newUser = make_document(
            kvp("username", username),
            kvp("password", password)
    );

    databaseConnection->insertUser(newUser);
}

void registerNewUser() {
    std::string newPassword;
    std::string confirmPassword;

    bool passwordsMatch = false;

    for (int passwordAttempts = 0; passwordAttempts < 3; ++passwordAttempts) {
        std::cout << "Welcome to Key Manager\n" << std::endl;
        std::cout << "Please set a password> ";

        std::cin >> newPassword;
        std::cout << std::endl;

        std::cout << "Please confirm the new password> ";

        std::cin >> confirmPassword;
        std::cout << std::endl;

        if (newPassword == confirmPassword) {
            passwordsMatch = true;
            addUserToDatabase(newPassword);
            break;
        } else {
            std::cout << "The Passwords must match. Please try again." << std::endl << std::endl;
        }
    }

    if(!passwordsMatch){
        std::cout << "User not registered after three failed set password attempts" << std::endl;
        exit(0);
    }
}

void checkPassword(std::string password) {
    //first we fetch the username so that we can query using it
    std::string username = getlogin();

    //mow we do the actual query to fetch the password
    std::string storedPassword = databaseConnection->findUserPassword(username);

    //then we compare the passwords.
    if(storedPassword == password){
        databaseConnection->addLog("Successful login conducted by user: " + username);
    } else {
        databaseConnection->addLog("Failed login attempt made by user: " + username);
        std::cout << "Incorrect Password." << std::endl;
    }

}

int main(int argc, char** argv) {

    databaseConnection = new DatabaseInterface();

    if(!databaseConnection->userRegistered()){
        registerNewUser();
    } else {
        std::string password = "p";
        checkPassword(password);
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







