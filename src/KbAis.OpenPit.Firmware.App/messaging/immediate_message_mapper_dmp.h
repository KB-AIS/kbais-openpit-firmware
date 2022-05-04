#ifndef IMMEDIATE_MESSAGE_MAPPER_DMP_H
#define IMMEDIATE_MESSAGE_MAPPER_DMP_H

// qt
#include <QtCore/qobject.h>
// oss
#include <rxcpp/rx-observable.hpp>

#include "IRxImmediateMessageMapper.h"
#include "state_watching/state_watcher_fuelling.h"

#include "opf/app/view/dmp/screens/main/main_view.h"

class DmpImmediateMessageMapper : public IRxImmediateMessageMapper {
    const main_view& main_view_;

    const state_watcher_fuelling& state_watcher_fuelling_;

public:
    DmpImmediateMessageMapper(
        const main_view& main_view
    ,   const state_watcher_fuelling& state_watcher_fuelling
    );

    rxcpp::observable<Message> getObservable() const override;

};

#endif // IMMEDIATE_MESSAGE_MAPPER_DMP_H
