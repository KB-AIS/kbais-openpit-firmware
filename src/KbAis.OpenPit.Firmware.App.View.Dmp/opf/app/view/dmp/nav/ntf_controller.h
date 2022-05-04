#ifndef NTF_CONTROLLER_H
#define NTF_CONTROLLER_H

// std
#include <functional>
// qt
#include <QtCore/qstring.h>
// oss
#include <rxcpp/rx.hpp>

struct ntf_prms {

    QString primary_text;

    QString secondary_text;

    std::optional<std::function<void()>> action_opt { std::nullopt };

};

class ntf_controller {

    rxcpp::rxsub::subject<ntf_prms> sub_ntf_;

public:
    void req_ntf(const ntf_prms&& prms) const;

    rxcpp::observable<ntf_prms> get_observable() const;

};

#endif // NTF_CONTROLLER_H
