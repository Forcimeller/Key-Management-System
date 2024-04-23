//
// Created by Jason Adoteye (M00659794)
// for CST3990 on 13/02/2024
//

#ifndef CMDLINE_MAIN_H
#define CMDLINE_MAIN_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include "DatabaseInterface.h"
#include "FileSystemInterface.h"
#include "unistd.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

#endif //CMDLINE_MAIN_H
