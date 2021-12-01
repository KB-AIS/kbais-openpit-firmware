#ifndef EVENT_H
#define EVENT_H

// Qt
#include <QString>
#include <QMetaType>

struct Event {

    const Event &operator=(const Event& event) {
        return { event.m_tag, event.m_payload };
    }

    const QString m_tag; // unique event tag

    const QString m_payload;

};

Q_DECLARE_METATYPE(Event);

#endif // EVENT_H
