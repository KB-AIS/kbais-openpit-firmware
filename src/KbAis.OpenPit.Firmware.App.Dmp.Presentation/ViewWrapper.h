#ifndef VIEWWRAPPER_H
#define VIEWWRAPPER_H

// qt
#include <QObject>
#include <QStackedWidget>

// cfw::infra::sensors::gps
#include "SerialRxGpsSensorPublisher.h"

#include "MainView.h"

class ViewWrapper : public QObject {
    Q_OBJECT

public:
    ViewWrapper(MainView& mainView);

    // Copy constructor to satisfy boost::di
    //ViewWrapper(const ViewWrapper& w) : QObject(), mainView { w.mainView } {}

    ~ViewWrapper();

    void navigateToMain();

    void navigateToDiag();

private:
    QStackedWidget* nav;

    MainView& mainView;
};

#endif // VIEWWRAPPER_H
