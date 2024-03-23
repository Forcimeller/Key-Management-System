#include "main.h"

void connectToDatabase();

int main() {

<<<<<<< HEAD
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
=======
    std::string word;

    std::cout << "Gimme a word any word!" << std::endl;
    std::cin >> word;


>>>>>>> master

    return 0;
}

