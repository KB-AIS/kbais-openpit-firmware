// Qt
#include <QApplication>
// Firmware Aux
#include <forms/main_form.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    main_form main_form;
    main_form.show();

    return app.exec();
}
