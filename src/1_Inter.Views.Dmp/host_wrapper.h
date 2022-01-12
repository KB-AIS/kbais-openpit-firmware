#ifndef HOST_WRAPPER_H
#define HOST_WRAPPER_H

// qt
#include <QObject>
#include <QStackedWidget>

#include "diag_presenter.h"
#include "SerialRxGpsSensorPublisher.h"
#include "main_presenter.h"

class HostWrapper : public QObject {
    Q_OBJECT

public:
    HostWrapper(const IRxGpsSensorPublisher&);

    // Copy constructor to satisfy boost::di
    HostWrapper(const HostWrapper&) : QObject() {}

    ~HostWrapper();

    void navigateToMain();

    void navigateToDiag();

    QStackedWidget* navigationStack;

    main_presenter* mainPresenter;

    diag_presenter* diagPresenter;

private:
    enum class NavStackIdx { main = 0, diag = 1, };
};

#endif // HOST_WRAPPER_H
