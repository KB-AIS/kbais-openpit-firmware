#ifndef VIEWWRAPPER_H
#define VIEWWRAPPER_H

// qt
#include <QObject>
#include <QStackedWidget>

#include "Diag/DiagView.h"
#include "MainView.h"
#include "NavigationEmmiter.h"
#include "SerialRxGpsSensorPublisher.h"

class ViewWrapper : public QObject {
    Q_OBJECT

public:
    ViewWrapper(MainView& mainView, DiagView& diagView, NavEmmiter& navigationEmmiter);

    ~ViewWrapper();

    void NavigateToMain();

    void NavigateToDiag();

private:
    QStackedWidget* m_nav;

    MainView& m_mainView;

    DiagView& m_diagView;

    NavEmmiter& m_navigationEmmiter;
};

#endif // VIEWWRAPPER_H
