 #include "database_factory.h"

// Qt
#include <QtSql>

const QString DB_DRIVER { "QSQLITE" };

const QString DB_NAME { "/media/app/cfw/cfw.db" };

void Caching::Configuration::configureConnection() {
    auto connection = QSqlDatabase::addDatabase(DB_DRIVER);

    connection.setDatabaseName(DB_NAME);
}
