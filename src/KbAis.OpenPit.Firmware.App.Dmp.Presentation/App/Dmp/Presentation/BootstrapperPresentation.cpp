#include "BootstrapperPresentation.h"

BootstrapperPresentation::BootstrapperPresentation(HostWindow& host_window) {
    Q_INIT_RESOURCE(opf_dmp);

    host_window.show();
}
