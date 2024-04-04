//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 03/03/2024
//

#include "DatabaseInterface.h"

/* Public: Constructor (just calls DatabaseInterface::connectToServer()) */
DatabaseInterface::DatabaseInterface() {
    connectToServer();
}

/* Public: Responsible for getting the password based on the user's username */
void DatabaseInterface::connectToServer() {

    auto databaseConnection = client[this->DATABASE_NAME];

    std::vector<std::string> databaseNames = client.list_database_names();

    for(std::string &databaseName : databaseNames){

        //if the database exists, then we can set new database to false. This
        //  will help avoid a segmentation fault
        if(databaseName == this->DATABASE_NAME) {
            this->newDatabase = false;
        }
    }


    this->database = databaseConnection;
}

/* Public: Responsible for determining whether the user exists */
bool DatabaseInterface::userRegistered(){

    //None of this is possible of the database doesn't exist.
    //  if the database doesn't exist, mongodb makes it when data is first added to it.
    //  we therefore wouldn't need to do all of these checks.
    if(this->newDatabase){
        return false;
    }

    //Here we fetch all the database names
    std::vector<std::string> collectionNames = this->database.list_collection_names();

    //Then we do a linear string match search
    for(std::string &collectionName : collectionNames){

        //if the collection exists, then a user must have been registered, therefore,
        //  return true
        if(collectionName == this->LOGIN_COLLECTION_NAME) {
            return true;
        }
    }
    return false;
}

/* Public: Responsible for adding the new user */
bool DatabaseInterface::insertUser(std::string password) {

    //fetching the user's username from the database
    std::string username = getlogin();

    //making the document which will be added to the database
    bsoncxx::v_noabi::document::view_or_value collectionEntry = make_document(
            kvp("username", username),
            kvp("password", password)
    );
    bool documentInserted = this->insertDocument(this->LOGIN_COLLECTION_NAME,
                                           collectionEntry);

    bool actionLogged = this->addLog("User Added to " + this->LOGIN_COLLECTION_NAME);

    return (documentInserted && actionLogged);
}

/* Public: Responsible for updating the user's password */
bool DatabaseInterface::changeUserPassword(std::string password) {

    //fetching the user's username from the database
    std::string username = getlogin();

    //making the criteria which we need to match before we add to the database
    bsoncxx::v_noabi::document::view_or_value searchCriterion = make_document(
            kvp("username", username)
    );

    //making the key-value pair which will replace an existing pair
    bsoncxx::v_noabi::document::view_or_value newPassword = make_document(
            kvp("password", password)
    );

    //execute
    bool documentUpdated = this->updateDocument(this->LOGIN_COLLECTION_NAME,
                                                searchCriterion,
                                                newPassword);

    bool actionLogged = this->addLog("User ("+ username +") updated their password.");

    return (documentUpdated && actionLogged);
}

/* Public: Responsible for getting the password based on the user's username */
std::string DatabaseInterface::findUserPassword(std::string username) {

    bsoncxx::v_noabi::document::view_or_value searchTerm =
            make_document(kvp("username", username));

    core::optional<bsoncxx::document::value> result =
            this->searchForSingleDocument(LOGIN_COLLECTION_NAME, searchTerm);

    bsoncxx::document::view resultDocument = result->view();

    bsoncxx::document::element passwordKvp = resultDocument["password"];

    std::string password = passwordKvp.get_value().get_string().value.to_string();
    return password;
}

/* Public: Responsible for adding logs */
bool DatabaseInterface::addLog(std::string logNote) {

    //Time of call
    auto dateTime = std::chrono::system_clock::now();

    //Declaration of the log
    bsoncxx::v_noabi::document::view_or_value newlog = make_document(
            kvp("description", logNote),
            kvp("datetime", bsoncxx::types::b_date(dateTime))
    );

    return this->insertDocument(this->LOG_COLLECTION_NAME,
                   newlog);
}

/* Private: Responsible for getting a single document using the given criteria */
core::optional<bsoncxx::document::value> DatabaseInterface::searchForSingleDocument
(std::string collectionName, bsoncxx::v_noabi::document::view_or_value searchCriteria){

    //Declaration of the target collection (table) to query
    auto collection = this->database[collectionName];

    core::optional<bsoncxx::document::value> result = collection.find_one(searchCriteria);

    return result;
}

/* Private: Responsible for getting a single document using the given criteria */
bool DatabaseInterface::insertDocument(std::string collectionName,
                                       bsoncxx::v_noabi::document::view_or_value collectionEntry) {

        //Declaration of the target collection (table) to add to
        auto collection = this->database[collectionName];

        //The actual insertion into the collection
        core::optional<mongocxx::result::insert_one>
                insertOneDocument = collection.insert_one(collectionEntry);

        return insertOneDocument.operator bool();
}

/* Private: Responsible for getting a single document using the given criteria */
bool DatabaseInterface::updateDocument
        (std::string collectionName,
         bsoncxx::v_noabi::document::view_or_value searchCriteria,
         bsoncxx::v_noabi::document::view_or_value updatedKvp){

    //Declaration of the target collection (table) to query
    auto collection = this->database[collectionName];

    bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> updateCommand =
            make_document(kvp("$set", updatedKvp));

    core::optional<mongocxx::result::update> result = collection.update_one(searchCriteria, updateCommand);

    return result.operator bool();
}

int DatabaseInterface::deleteDocument() {
    return 0;
}





