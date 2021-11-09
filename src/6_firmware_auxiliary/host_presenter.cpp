#include "host_presenter.h"

enum class NavStackIdx {
    Main = 0,
    Diag = 1,
};

host_presenter::host_presenter(QObject* parent) : QObject(parent),
    nav_stack(),
    diag_pre(),
    main_pre() {

    nav_stack.insertWidget(static_cast<int>(NavStackIdx::Main), main_pre);
    nav_stack.insertWidget(static_cast<int>(NavStackIdx::Diag), diag_pre);

    // nav_stack.show();
}

void host_presenter::nav_to_main() {
    nav_stack.setCurrentIndex(static_cast<int>(NavStackIdx::Main));
}

void host_presenter::nav_to_diag() {
    nav_stack.setCurrentIndex(static_cast<int>(NavStackIdx::Diag));
}
