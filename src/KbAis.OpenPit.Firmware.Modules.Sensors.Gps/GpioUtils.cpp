#include "GpioUtils.h"

// qt
#include <QDir>

const QDir GPIO_DIRECTORY { "/sys/class/gpio" };

const QString GPIO_FILE_EXP { QStringLiteral("export") };

const QString GPIO_FILE_DIR { QStringLiteral("/gpio%1/direction") };

const QString GPIO_FILE_VAL { QStringLiteral("/gpio%1/value") };

void
GpioUtils::export_pin(int pin) {
    QFile file_exp { GPIO_DIRECTORY.filePath(GPIO_FILE_EXP) };

    const auto opened = file_exp.open(QIODevice::WriteOnly);
    if (!opened) return;

    file_exp.write(QString::number(pin).toUtf8());
    file_exp.close();
}

void
GpioUtils::setup_dir(int pin, PinDirection dir) {
    const auto path { GPIO_DIRECTORY.path() + GPIO_FILE_DIR.arg(pin) };
    QFile file_dir { path };

    const auto opened = file_dir.open(QIODevice::WriteOnly);
    if (!opened) return;

    file_dir.write(dir == PinDirection::In ? "in" : "out");
    file_dir.close();
}

void
GpioUtils::setup_val(int pin, bool val) {   
    const auto path { GPIO_DIRECTORY.path() + GPIO_FILE_VAL.arg(pin) };
    QFile file_val { path };

    const auto opened = file_val.open(QIODevice::WriteOnly);
    if (!opened) return;

    file_val.write(val ? "1" : "0");
    file_val.close();
}
