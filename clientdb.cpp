#include "clientdb.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>

ClientDB* ClientDB::Open(const QString& path)
{
	for (QString str : QSqlDatabase::drivers())
		qDebug() << str;

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	if (!db.isValid())
	{
		qDebug() << "Can't open database: " << db.lastError();
		db.close();
		return nullptr;
	}
	db.setDatabaseName(":memory:");
	db.setUserName("");
	db.setPassword("");
	db.setHostName("???");
	db.setPort(0);
	db.setConnectOptions("???");
	db.open();
	if (!db.isValid())
	{
		qDebug() << "Can't open database: " << db.lastError();
		db.close();
		return nullptr;
	}

	return new ClientDB(db);
}

ClientDB::ClientDB(QSqlDatabase db)
{
	m_db = db;
}

ClientDB::~ClientDB()
{
	m_db.close();
}
