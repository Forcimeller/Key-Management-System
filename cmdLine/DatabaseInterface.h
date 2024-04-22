//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 03/03/2024
//
#ifndef CMDLINE_DATABASEINTERFACE_H
#define CMDLINE_DATABASEINTERFACE_H

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <csignal>
#include <cassert>
#include <chrono>
#include <ctime>
#include <vector>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

class DatabaseInterface {
public:
    //Public fields & Method Declarations
    struct LogEntry {
        std::string logDate;
        std::string logContent;
    };

    struct KeyEntry {
        std::string keyName;
        std::string extendedKeySample;
    };

    DatabaseInterface();
    ~DatabaseInterface();
    bool userRegistered();
    bool insertUser(std::string password);
    bool insertKey(std::string keyName, std::string key);
    bool changeUserPassword(std::string password);
    bool addLog(std::string logNote);
    bool updateKey(std::string keyName, std::string key);
    bool deleteKey(std::string keyName);
    std::vector<LogEntry> getAllLogs();
    std::vector<KeyEntry> getAllKeys();
    std::string findUserPassword(std::string username);
    std::string findKey(std::string keyName);

private:
    //Private fields & Method Declarations
    const mongocxx::instance instance = {};
    const mongocxx::client client = {mongocxx::uri{}};
    mongocxx::database database;

    bool newDatabase = true;

    const std::string DATABASE_NAME = "KeyManBase";
    const std::string LOGIN_COLLECTION_NAME = "login";
    const std::string KEY_COLLECTION_NAME = "keys";
    const std::string LOG_COLLECTION_NAME = "logs";

    void connectToServer();
    core::optional<bsoncxx::document::value>
                searchForSingleDocument(std::string collectionName,
                                        bsoncxx::v_noabi::document::view_or_value searchCriteria);

    mongocxx::cursor searchForMultipleDocuments(std::string collectionName,
                                                bsoncxx::v_noabi::document::view_or_value searchCriteria);
    bool insertDocument(std::string collectionName,
                        bsoncxx::v_noabi::document::view_or_value collectionEntry);
    bool updateDocument(std::string collectionName,
                        bsoncxx::v_noabi::document::view_or_value searchCriteria,
                        bsoncxx::v_noabi::document::view_or_value updatedKvp);
    bool deleteSingleDocument(std::string collectionName, bsoncxx::v_noabi::document::view_or_value searchCriteria);
    bool documentExists(std::string key, std::string value, std::string collection);
    bool collectionExists(std::string collectionToFind);

    std::string getExistingKeyName(std::string &key);
    std::string dateTimeToString(bsoncxx::document::element datetimeElement);
};

#endif //CMDLINE_DATABASEINTERFACE_H
