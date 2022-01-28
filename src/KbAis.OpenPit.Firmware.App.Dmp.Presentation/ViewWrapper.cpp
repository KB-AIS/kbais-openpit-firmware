 #include "ViewWrapper.h"

constexpr int SCREEN_W { 1024 }, SCREEN_H { 640 };

constexpr int MAIN_VIEW_IDX { 0 };

ViewWrapper::ViewWrapper(MainView& mainView)
    : nav { new QStackedWidget }
    , mainView { mainView }
{
    nav->setGeometry(0, 0, SCREEN_W, SCREEN_H);
    nav->setStyleSheet("QWidget { background-color: black; color: white }");

    nav->insertWidget(MAIN_VIEW_IDX, &mainView);

    nav->setCurrentIndex(MAIN_VIEW_IDX);
    nav->show();
}

ViewWrapper::~ViewWrapper() {}

void ViewWrapper::navigateToMain() {
    nav->setCurrentIndex(MAIN_VIEW_IDX);
}

void ViewWrapper::navigateToDiag() {}
