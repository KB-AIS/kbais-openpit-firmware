#ifndef DMPIMMEDIATEMESSAGEMAPPER_H
#define DMPIMMEDIATEMESSAGEMAPPER_H

// qt
#include <QObject>

#include "App/Dmp/Presentation/Views/Main/MainView.h"
#include "IRxImmediateMessageMapper.h"
#include "RxQt.h"
#include "state_watcher_fuelling.h"

class DmpImmediateMessageMapper : public IRxImmediateMessageMapper {
    const MainView& m_main_view;

    const StateWatcherFuelling& m_state_ful_pub;

public:
    DmpImmediateMessageMapper(
        const MainView& main_view
    ,   const StateWatcherFuelling& state_ful_pub
    );

    rxcpp::observable<Message> getObservable() const override;

};

#endif // DMPIMMEDIATEMESSAGEMAPPER_H
