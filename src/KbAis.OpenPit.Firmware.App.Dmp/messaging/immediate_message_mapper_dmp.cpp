#include "immediate_message_mapper_dmp.h"

#include "Mappers/JsonMappers.h"
#include "QtExtensions/QByteArrayExt.h"
#include "RxQt.h"

const QString MESSAGE_MONKIER_USR { QStringLiteral("USR") };

const QString MESSAGE_MONKIER_STA { QStringLiteral("STA") };

DmpImmediateMessageMapper::DmpImmediateMessageMapper(
    const main_view& main_view
,   const state_watcher_fuelling& state_ful_pub
)
    :   m_main_view(main_view)
    ,   m_state_ful_pub(state_ful_pub)
{

}

rxcpp::observable<Message>
DmpImmediateMessageMapper::getObservable() const {
    rxcpp::observable<Message> observable = rxcpp::observable<>::empty<Message>();

//    rxcpp::observable<Message> user_message_observable =
//        rxqt::from_signal(&m_main_view, &main_view::notify_test_user_event)
//            .map([&](auto) -> Message {
//                return Message {
//                    MESSAGE_MONKIER_USR
//                ,   fromStdVector(nlohmann::json::to_msgpack("Some user's UI event"))
//                ,   QDateTime::currentDateTimeUtc()
//                };
//            });

    rxcpp::observable<Message> state_fueling_message_observable =
        m_state_ful_pub.get_observable()
            .map([&](const state_changed_message& x) -> Message {
                nlohmann::json j_object;
                j_object["COD"] = x.new_state_code;

                return Message {
                    MESSAGE_MONKIER_STA
                ,   fromStdVector(nlohmann::json::to_msgpack(j_object))
                ,   QDateTime::currentDateTimeUtc()
                };
            });

    return observable.merge(/*user_message_observable, */state_fueling_message_observable);
}
