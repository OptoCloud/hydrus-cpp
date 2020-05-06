#ifndef CLIENTDB_H
#define CLIENTDB_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class ClientDB : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ClientDB)

	ClientDB();
public:
	static ClientDB* Open(const QString& path);
	~ClientDB();
private:

	QSqlDatabase m_db;
};

#endif // CLIENTDB_H
