#include "clientdb.h"

#include <iostream>
ClientDB::ClientDB(const std::string& dbPath)
{
    std::cout << sqlite3_open(dbPath.c_str(), &m_db) << std::endl;
    std::cout << sqlite3_close(m_db) << std::endl;
}
