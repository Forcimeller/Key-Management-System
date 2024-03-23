cmake_minimum_required(VERSION 3.27)
project(cmdLine)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

find_library(MONGOCXX_LIBRARIES NAMES mongocxx PATHS /usr/local/lib)
find_library(BSONCXX_LIBRARIES NAMES bsoncxx PATHS /usr/local/lib)

add_executable(cmdLine main.cpp
        main.h)

target_link_libraries(cmdLine ${MONGOCXX_LIBRARIES} ${BSONCXX_LIBRARIES})