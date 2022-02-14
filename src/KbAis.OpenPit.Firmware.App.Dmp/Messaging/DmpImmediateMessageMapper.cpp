#include "DmpImmediateMessageMapper.h"

#include "Mappers/JsonMappers.h"
#include "QtExtensions/QByteArrayExt.h"
#include "RxQt.h"

const QString MESSAGE_MONKIER_USR { QStringLiteral("USR") };

DmpImmediateMessageMapper::DmpImmediateMessageMapper(const MainView& mainView)
    :   mMainView { mainView }
{

}

rxcpp::observable<Message>
DmpImmediateMessageMapper::getObservable() const {
    return rxqt::from_signal(&mMainView, &MainView::notifyTestUserEvent)
        .map([&](auto) {
            return Message {
                MESSAGE_MONKIER_USR
            ,   fromStdVector(nlohmann::json::to_msgpack("Some user's UI event"))
            ,   QDateTime::currentDateTimeUtc()
            };
        });
}
