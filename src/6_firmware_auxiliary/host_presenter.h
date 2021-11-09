#ifndef HOST_PRESENTER_H
#define HOST_PRESENTER_H

#include <presentation/diag_presenter.h>
#include <presentation/main_presenter.h>

// qt
#include <QObject>
#include <QStackedWidget>

class host_presenter : public QObject {
    Q_OBJECT

public:
    explicit host_presenter(QObject* parent = nullptr);

    /*!
     * \brief navigate_to_main
     */
    void nav_to_main();

    /*!
     * \brief navigate_to_diag
     */
    void nav_to_diag();

private:
    QStackedWidget nav_stack;

    diag_presenter* diag_pre;

    main_presenter* main_pre;

};

#endif // HOST_PRESENTER_H
