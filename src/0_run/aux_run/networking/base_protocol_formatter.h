#ifndef BASE_PROTOCOL_FORMATTER_H
#define BASE_PROTOCOL_FORMATTER_H

// qt
#include <QByteArray>
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QUuid>

namespace KbAis::Cfw::Networking {

enum class DataType : quint8 {
    ATH = 0x00,
    TEL = 0x01,
    ACK = 0xFF,
};

/*!
 * \brief The AthData struct
 */
struct AthData {

    /*!
     * \brief equipmentId
     */
    QString equipmentId;

};

/*!
 * \brief The AckData struct
 */
struct AcknData {

    /*!
     * \brief uuid
     */
    QUuid uuid;

    /*!
     * \brief result
     */
    quint8 result;

};

/*!
 * \brief The TelData struct
 */
struct TelData {

};

struct ProtocolFormatterNotifier : public QObject {
    Q_OBJECT

    Q_SIGNAL void notifyPackageAccepted();

    Q_SIGNAL void notifyPackageRejected();

};

/*!
 * \brief The BaseProtocolFormatter class
 */
class BaseProtocolFormatter {

public:
    // BaseProtocolFormatter(const ProtocolFormatterNotifier& notifier);

    // TODO: Pass specific formatter configuration?
    virtual QByteArray encodeAuthPackage(const QString& equipmentId) = 0;

    // virtual QByteArray encodeTelePackage() = 0;

//private:
//    const ProtocolFormatterNotifier& notifier;

};

/*!
 * \brief The SwomProtocolFromatter class
 */
class SwomProtocolFromatter : public virtual BaseProtocolFormatter {

public:
    // SwomProtocolFromatter(const ProtocolFormatterNotifier& notifier);

    QByteArray encodeAuthPackage(const QString &equipmentId);

    // QByteArray encodeTelePackage();

};

}

#endif // BASE_PROTOCOL_FORMATTER_H
