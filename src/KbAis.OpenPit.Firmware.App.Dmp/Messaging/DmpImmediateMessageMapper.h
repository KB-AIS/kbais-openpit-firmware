#ifndef DMPIMMEDIATEMESSAGEMAPPER_H
#define DMPIMMEDIATEMESSAGEMAPPER_H

// qt
#include <QObject>

#include "App/Dmp/Presentation/Views/Main/MainView.h"
#include "IRxImmediateMessageMapper.h"
#include "RxQt.h"

class DmpImmediateMessageMapper : public IRxImmediateMessageMapper {

public:
    DmpImmediateMessageMapper(const MainView& mainView);

    rxcpp::observable<Message> getObservable() const override;

private:
    const MainView& mMainView;

};

#endif // DMPIMMEDIATEMESSAGEMAPPER_H
