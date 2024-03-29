//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 03/03/2024
//

#include "DatabaseInterface.h"

DatabaseInterface::DatabaseInterface() {
    connectToServer();
}

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

int DatabaseInterface::searchDatabase() {
    return 0;
}

bool DatabaseInterface::insertUser(bsoncxx::v_noabi::document::view_or_value collectionEntry) {

    bool documentInserted = this->insertDocument(this->LOGIN_COLLECTION_NAME,
                                           collectionEntry);

    bool actionLogged = this->addLog("User Added to " + this->LOGIN_COLLECTION_NAME);

    return (documentInserted && actionLogged);
}

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

int DatabaseInterface::deleteEntry() {
    return 0;
}

void DatabaseInterface::searchForDocument() {

}

bool DatabaseInterface::insertDocument(std::string collectionName,
                                       bsoncxx::v_noabi::document::view_or_value collectionEntry) {

        //Declaration of the target collection (table) to add to
        auto collection = this->database[collectionName];

        //The actual insertion into the collection
        //core::optional<mongocxx::result::insert_one> insert_one_result;
        //insert_one_result =

        collection.insert_one(collectionEntry);

    //bsoncxx::types::bson_value::view doc_id = insert_one_result->inserted_id();
    //assert(insert_one_result);

    return true; //insert_one_result.operator bool();
}

int DatabaseInterface::deleteDocument() {
    return 0;
}



