#ifndef DMPIMMEDIATEMESSAGEMAPPER_H
#define DMPIMMEDIATEMESSAGEMAPPER_H

// qt
#include <QObject>

// App.Dmp.Presentation
#include "MainView.h"
// Core.Messaging
#include "IRxImmediateMessageMapper.h"
// Utils.TrdParty.RxQt
#include "RxQt.h"

class DmpImmediateMessageMapper : public IRxImmediateMessageMapper {

public:
    DmpImmediateMessageMapper(const MainView& mainView);

    rxcpp::observable<Message> getObservable() const override;

private:
    const MainView& mMainView;

};

#endif // DMPIMMEDIATEMESSAGEMAPPER_H
