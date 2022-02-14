 #include "ViewWrapper.h"

// qt
#include <QThread>

constexpr qint32 SCREEN_W { 1024 }, SCREEN_H { 640 };

constexpr qint32 MAIN_VIEW_IDX { 0 }, DIAG_VIEW_IDX { 1 };

ViewWrapper::ViewWrapper(
    MainView& mainView
,   DiagView& diagView
,   NavEmmiter& navigationEmmiter
)
    :   m_runLoopMain { this }
    ,   m_nav { new QStackedWidget }
    ,   m_mainView { mainView }
    ,   m_diagView { diagView }
    ,   m_navigationEmmiter { navigationEmmiter }
{
    m_nav->setGeometry(0, 0, SCREEN_W, SCREEN_H);
    m_nav->setStyleSheet(R"(
        QWidget {
            background-color: rgb(40, 40, 40);
            color: rgb(235, 219, 178);
        }
    )");

    m_mainView.setParent(m_nav);
    m_nav->insertWidget(MAIN_VIEW_IDX, &m_mainView);

    m_diagView.setParent(m_nav);
    m_nav->insertWidget(DIAG_VIEW_IDX, &m_diagView);

    m_nav->setCurrentIndex(MAIN_VIEW_IDX);
    m_nav->show();

    m_navigationEmmiter.GetObservableNavRequested()
        .subscribe([&](int viewIdx) {
            switch (viewIdx) {
            case MAIN_VIEW_IDX:
                NavigateToMain();
                break;
            case DIAG_VIEW_IDX:
                NavigateToDiag();
                break;
            }
        });

    m_mainView.StartObserveOn(m_runLoopMain.observe_on_run_loop());
}

ViewWrapper::~ViewWrapper() {

}

void
ViewWrapper::NavigateToMain() {
    m_nav->setCurrentIndex(MAIN_VIEW_IDX);
}

void
ViewWrapper::NavigateToDiag() {
    m_nav->setCurrentIndex(DIAG_VIEW_IDX);
}
