#include "clientdb.h"

#include <QDebug>
ClientDB::ClientDB(const char* dbPath)
{
    qDebug() << sqlite3_open(dbPath, &m_db);
    qDebug() << sqlite3_close(m_db);
}
