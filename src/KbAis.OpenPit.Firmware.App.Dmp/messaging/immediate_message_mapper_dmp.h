#ifndef IMMEDIATE_MESSAGE_MAPPER_DMP_H
#define IMMEDIATE_MESSAGE_MAPPER_DMP_H

// qt
#include <QObject>

#include "IRxImmediateMessageMapper.h"
#include "RxQt.h"
#include "opf/app/view/dmp/screens/main/main_view.h"
#include "state_watching/state_watcher_fuelling.h"

class DmpImmediateMessageMapper : public IRxImmediateMessageMapper {
    const main_view& m_main_view;

    const state_watcher_fuelling& m_state_ful_pub;

public:
    DmpImmediateMessageMapper(
        const main_view& main_view
    ,   const state_watcher_fuelling& state_ful_pub
    );

    rxcpp::observable<Message> getObservable() const override;

};

#endif // IMMEDIATE_MESSAGE_MAPPER_DMP_H
