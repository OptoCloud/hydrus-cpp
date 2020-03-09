#include "clientdb.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

const QString DRIVER("QSQLITE");

ClientDB* ClientDB::Open(const QString& path)
{
	if(!QSqlDatabase::isDriverAvailable(DRIVER))
		return nullptr;

	QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
	db.setDatabaseName(path);
	db.open();

	if (!db.open())
	{
		qDebug() << "Can't open database: " << db.lastError();
		return nullptr;
	}

	return new ClientDB(db);
}

ClientDB::ClientDB(const QSqlDatabase& db)
{
	m_db = db;
}

ClientDB::~ClientDB()
{
	m_db.close();
}
