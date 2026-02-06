#include "../headers/DataBase.hpp"

DataBase::DataBase(){
    _error = sqlite3_open(_file_name.c_str(), &_db);
    if(_error){
        std::cout << "Error opening the database:" << _error << std::endl;
        sqlite3_close(_db);
    }
}

DataBase::DataBase(const std::string& file_name){
    _file_name = file_name;
    _error = sqlite3_open(_file_name.c_str(), &_db);
    if(_error){
        std::cout << "Error opening the database:" << _error << std::endl;
        sqlite3_close(_db);
    }
}

DataBase::~DataBase(){
    sqlite3_close(_db);
    delete _error_message;
}


int _callback(void* none, int count, char** names, char** col_names){
    for(int i = 0; i < count; ++i){
        
    }

    return 0;
}


void DataBase::select_all(const std::string& table_name){
    _querry = "SELECT * FROM " + table_name;
    _error = sqlite3_exec(_db, _querry.c_str(), _callback, nullptr, &_error_message);
    _check_error("Selecting");
}

void DataBase::_download_all_table_names(){
    _querry = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
    _error = sqlite3_exec(_db, _querry.c_str(), _callback, nullptr, &_error_message);
    _check_error("Downloading all table names");
}

void DataBase::_check_error(const std::string& action){
    if(_error != SQLITE_OK){
        std::cout << action << " table ended with an error: " << _error << std::endl;
        std::cout << "Error message: " << _error_message << std::endl;
        sqlite3_free(_error_message);
    }
}

void DataBase::init_db(){
    _download_all_table_names();

}
