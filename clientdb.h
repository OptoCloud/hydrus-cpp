#ifndef CLIENTDB_H
#define CLIENTDB_H

#include <string>
#include <SQLite/sqlite3.h>

class ClientDB
{
public:
    ClientDB(const std::string& dbPath);
private:
    sqlite3* m_db = nullptr;
};

#endif // CLIENTDB_H
