#ifndef INFO_VIEW_H
#define INFO_VIEW_H

#include <QtWidgets/qwidget.h>

#include "opf/app/view/dmp/nav/nav_controller.h"
#include "opf/app/view/dmp/screens/stacked_view.h"

namespace Ui { class info_view; }

class info_view : public QWidget, public i_stacked_view {
    Q_OBJECT

    Ui::info_view* ui_;

    const nav_controller& nav_controller_;

public:
    explicit info_view(const nav_controller& nav_controller);

    ~info_view();

    int get_view_idx() const noexcept override;
};

#endif // INFO_VIEW_H
