#include "clientdb.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

const QString DRIVER("QSQLITE");

ClientDB::ClientDB(){}

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

	auto cdb = new ClientDB();
	cdb->m_db = db;
	return cdb;
}

ClientDB::~ClientDB()
{
	m_db.close();
}
