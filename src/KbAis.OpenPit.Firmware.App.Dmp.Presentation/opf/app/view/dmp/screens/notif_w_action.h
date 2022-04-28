#ifndef NOTIF_W_ACTION_H
#define NOTIF_W_ACTION_H

// std
#include <functional>
// qt
#include <QWidget>

namespace Ui { class notif_w_action; }

class notif_w_action : public QWidget {
    Q_OBJECT

    Ui::notif_w_action* ui_;

public:
    explicit notif_w_action(QWidget* parent = nullptr);

    ~notif_w_action();

    void show_notification(const std::string& text, std::function<void()> action);

};

#endif // NOTIF_W_ACTION_H
