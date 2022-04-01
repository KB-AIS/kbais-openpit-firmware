 #include "DatabaseConfigurator.h"

// qt
#include <QtSql>

QString DB_DRIVER { "QSQLITE" };

QString DB_NAME { "/media/app/cfw/cfw.db" };

void
DatabaseConfigurator::configure() {
    auto connection = QSqlDatabase::addDatabase(DB_DRIVER);

    connection.setDatabaseName(DB_NAME);
}
