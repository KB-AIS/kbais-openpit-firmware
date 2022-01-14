 #include "DatabaseConfigurator.h"

// qt sql
#include <QtSql>

const QString DB_DRIVER { "QSQLITE" };

const QString DB_NAME { "/media/app/cfw/cfw.db" };

void
DatabaseConfigurator::configure() {
    auto connection = QSqlDatabase::addDatabase(DB_DRIVER);

    connection.setDatabaseName(DB_NAME);
}
