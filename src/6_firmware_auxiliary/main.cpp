#include <iostream>

#include <QApplication>

#include <widgets/host_widget.h>

#include <hello_world.h>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    host_widget host_widget;
    host_widget.show();

    auto foo = answer_of_life();
    foo++;

    return app.exec();
}
