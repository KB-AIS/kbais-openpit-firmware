#include <iostream>

#include <QApplication>

#include <widgets/main_widget.h>

#include <hello_world.h>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    host_widget host_widget;
    host_widget.show();

    return app.exec();
}
