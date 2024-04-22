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

/* Public: Responsible for adding keys to the database */
bool DatabaseInterface::insertKey(std::string keyName, DatabaseInterface::Key key) {

    //Collection existence check to prevent errors.
    //Accept keys if the "keys" collection doesn't exist
    if (this->collectionExists(KEY_COLLECTION_NAME)) {

        //Decline to accept key if the key name already exists.
        if (documentExists("keyName", keyName, KEY_COLLECTION_NAME)) {
            std::cout << "You already have a key with that name. All key names need to be unique." << std::endl;
            this->addLog("User attempted to add a key using an existing key name");
            return false;

        //Decline if Key already exists, give user the name of the key
        } else if (documentExists("key", key.keyContents, KEY_COLLECTION_NAME)) {

            std::string keyNameResult = getExistingKeyName(key.keyContents);

            //Message to user
            std::cout << "You have already added this key. It has been saved as \""
                      << keyNameResult << "\"." << std::endl;
            this->addLog("User attempted to add an existing key.");

            return false;
        }
    }

    //making the document which will be added to the database
    bsoncxx::v_noabi::document::view_or_value collectionEntry = make_document(
            kvp("keyName", keyName),
            kvp("keySample",key.keyContents.substr(36, 5)),  // a sample of the key after the header
            kvp("keyType", key.keyType),
            kvp("key", key.keyContents)
    );
    bool documentInserted = this->insertDocument(this->KEY_COLLECTION_NAME,
                                                 collectionEntry);

    bool actionLogged = this->addLog("New Key " + key.keyContents.substr(36, 5) + " added to database.");
    std::cout << "Key added to database" << std::endl;

    return (documentInserted && actionLogged);
}

/* Public: Responsible for updating replacing the key stored under the provided key name */
bool DatabaseInterface::updateKey(std::string keyName, DatabaseInterface::Key key) {
    //Collection existence check to prevent errors.
    //Accept keys if the "keys" collection doesn't exist
    if (this->collectionExists(KEY_COLLECTION_NAME)) {

        //Decline if Key already exists, give user the name of the key
        if (documentExists("key", key.keyContents, KEY_COLLECTION_NAME)) {

        std::string keyNameResult = getExistingKeyName(key.keyContents);

        //Message to user
        std::cout << "You have already added this key. It has been saved as \""
                  << keyNameResult << "\"." << std::endl;
        this->addLog("User attempted to Update a key with another existing key.");

        return false;

        //Accept key if the key name exists.
        } else if (documentExists("keyName", keyName, KEY_COLLECTION_NAME)) {

            //making the document which will be added to the database
            bsoncxx::v_noabi::document::view_or_value searchCriteria = make_document(
                    kvp("keyName", keyName)
            );

            //making the document which will be added to the database
            bsoncxx::v_noabi::document::view_or_value newKey = make_document(
                    kvp("keySample", key.keyContents.substr(36, 5)),
                    // ^a sample of the key after the header

                    kvp("keyType", key.keyType),
                    kvp("key", key.keyContents)
            );

            //Execute update
            bool documentInserted = this->updateDocument(KEY_COLLECTION_NAME,
                                                         searchCriteria,
                                                         newKey);

            //message to user
            std::cout << "Key " << keyName << " updated with new key "
                      << key.keyContents.substr(36, 5) << "." << std::endl;

            // Log event
            bool actionLogged = this->addLog("User updated key \"" + keyName + "\" with new key "
                    + key.keyContents.substr(36, 5) + ".");

            return (documentInserted && actionLogged);
        }
    }
    // Reject key if key name does not exist
    std::cout << "The key " << keyName << " does not exist." << std::endl;
    this->addLog("User attempted to add a key using a key name which did not exist");
    return false;
}

/* Public: Responsible for updating existing keys */
bool DatabaseInterface::deleteKey(std::string keyName) {

    //query filter for the key
    bsoncxx::v_noabi::document::view_or_value searchTerm = make_document(
            kvp("keyName", keyName)
    );

    //Delete key
    bool documentDeleted = deleteSingleDocument(KEY_COLLECTION_NAME, searchTerm);

    //Log it
    bool actionLogged = this->addLog("Key \"" + keyName + "\" was deleted from the database.");

    return documentDeleted && actionLogged;
}

/* Public: Responsible for getting keys from the database. */
DatabaseInterface::Key DatabaseInterface::findKey(std::string keyName) {

    //query filter for the key
    bsoncxx::v_noabi::document::view_or_value searchTerm = make_document(
            kvp("keyName", keyName)
    );

    //Querying the database
    core::v1::optional<bsoncxx::document::value> result =
            searchForSingleDocument(KEY_COLLECTION_NAME,
                                    searchTerm);

    if(result->begin() == result->end()){
        std::cout << "No key with that name could be found. "
                  << "Use --seeKeys to see all keys or "
                  << "use --seeLogs to see if any changes have taken place. "
                  << std::endl;

        exit(0);
    }

    //selecting the key name and type values
    bsoncxx::document::view resultDocument = result->view();
    bsoncxx::document::element keyKvp = resultDocument["key"];
    std::string keyResult = keyKvp.get_value().get_string().value.to_string();

    bsoncxx::document::element typeKvp = resultDocument["keyType"];
    std::string typeResult = typeKvp.get_value().get_string().value.to_string();
    return {keyResult, typeResult};
}

/* Public: responsible for fetching all logs from the database */
std::vector<DatabaseInterface::LogEntry> DatabaseInterface::getAllLogs(){

    mongocxx::cursor logQuery = searchForMultipleDocuments(LOG_COLLECTION_NAME,
                                                           {});
    std::vector<DatabaseInterface::LogEntry> logVector;
    for(auto entry : logQuery){
        logVector.push_back({
            dateTimeToString(entry["datetime"]),
            entry["description"].get_value().get_string().value.to_string()
        });
    }

    return logVector;
}

/* Public: responsible for fetching all keys from the database */
std::vector<DatabaseInterface::KeyEntry> DatabaseInterface::getAllKeys(){
    mongocxx::cursor keyQuery = searchForMultipleDocuments(KEY_COLLECTION_NAME,
                                                           {});
    std::vector<DatabaseInterface::KeyEntry> keyVector;
    for(auto entry : keyQuery){
        keyVector.push_back({
            entry["keyName"].get_value().get_string().value.to_string(),
            entry["key"].get_value().get_string().value.to_string().substr(36, 15),
        });
    }

    return keyVector;
}

/* Private: responsible for getting the name via key. Extracted from insertKey() */
std::string DatabaseInterface::getExistingKeyName(std::string &key) {
    //query filter for the key
    bsoncxx::v_noabi::document::view_or_value searchTerm = make_document(
            kvp("key", key)
    );

    //Querying the database
    core::v1::optional<bsoncxx::document::value> result =
            searchForSingleDocument(KEY_COLLECTION_NAME,
                                    searchTerm);
    //selecting the key name value
    bsoncxx::document::view resultDocument = result->view();
    bsoncxx::document::element keyNameKvp = resultDocument["keyName"];
    std::string keyNameResult = keyNameKvp.get_value().get_string().value.to_string();
    return keyNameResult;
}

/* Private: Blackbox responsible for parsing datetimes from the database  */
std::string DatabaseInterface::dateTimeToString(bsoncxx::document::element datetimeElement){

    //Casting the datetime as a milliseconds offset
    std::chrono::milliseconds milliseconds(datetimeElement.get_date().to_int64());

    //casting that offset into a time point
    std::chrono::system_clock::time_point timePoint
    (std::chrono::duration_cast<std::chrono::system_clock::duration>(milliseconds));
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm localTime = *std::localtime(&time);

    //Formatting the time using an object stream
    std::ostringstream dateTimeStream;
    dateTimeStream << std::put_time(&localTime, "%d/%m/%Y %H:%M:%S");

    return dateTimeStream.str();
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

    //Log this event
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

/* Private: Responsible for fetching a selection of documents */
mongocxx::cursor DatabaseInterface::searchForMultipleDocuments
    (std::string collectionName, bsoncxx::v_noabi::document::view_or_value searchCriteria) {


    //Declaration of the target collection (table) to query
    auto collection = this->database[collectionName];

    mongocxx::cursor results = collection.find(searchCriteria);

    return results;

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

    //the command that will be added to the virtual shell to find and update the specified kvp
    bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> updateCommand =
            make_document(kvp("$set", updatedKvp));

    //Execute
    core::optional<mongocxx::result::update> result = collection.update_one(searchCriteria, updateCommand);

    return result.operator bool();
}

/* Private: Responsible for removing documents form the database */
bool DatabaseInterface::deleteSingleDocument(std::string collectionName,
                                             bsoncxx::v_noabi::document::view_or_value searchCriteria){

    //Declaration of the target collection (table) to query
    auto collection = this->database[collectionName];

    //Execute
    core::optional<mongocxx::result::delete_result> result = collection.delete_one(searchCriteria);

    return result.operator bool();
}

/* Private: Responsible for determining whether the number of results returned is zero*/
bool DatabaseInterface::documentExists(std::string key,
                                       std::string value,
                                       std::string collection){
    //Create search term from given arguments
    bsoncxx::v_noabi::document::view_or_value searchTerm =
            make_document(kvp(key, value));

    //Execute query
    mongocxx::cursor result =
            this->searchForMultipleDocuments(collection, searchTerm);

    //If the starting point is the same as the end point, then there are no results.
    if (result.begin() == result.end()){
        return false;
    } else {
        return true;
    }
}

/* Private: Responsible for checking if a collection has been made */
bool DatabaseInterface::collectionExists(std::string collectionToFind) {

    //fetch all collections
    auto collections = database.list_collection_names();

    //loop through them in search for the specified collection
    for(auto&& name : collections){
        //End Loop if found
        if(collectionToFind == name){
            return true;
        }
    }

    return false;
}









