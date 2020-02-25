#ifndef CLIENTDB_H
#define CLIENTDB_H

#include <QtSql/QSqlDatabase>
class ClientDB
{
public:
	static ClientDB* Open(const QString& path);
	~ClientDB();
private:
	ClientDB() = delete;
	ClientDB(const QSqlDatabase& db);

	QSqlDatabase m_db;
};

#endif // CLIENTDB_H
