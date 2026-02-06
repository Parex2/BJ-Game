#pragma once
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <any>
#include <map>
#include "important.hpp"

class DataBase {
    sqlite3* _db = nullptr;
    std::string _file_name {"db.db"};
    int _error;
    std::string _querry;
    char* _error_message;

    std::map<std::string, std::vector<std::any>> _table_names;

    static int _callback(void*, int, char**, char**);
    void _check_error(const std::string&);
    void _download_all_table_names();


    public:
    
    DataBase();
    DataBase(const std::string&);
    ~DataBase();

    /**
     * @brief Prints all rows from a specific table
     * 
     * 
     */
    void select_all(const std::string&);
    void init_db();
};