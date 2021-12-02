#ifndef EVENT_H
#define EVENT_H

// Qt
#include <QString>
#include <QMetaType>

struct Event {
    /*!
     * Group events by their type.
     */
    const QString tag;

    const QString payload;

    const Event &operator=(const Event& event) {
        return { event.tag, event.payload };
    }

};

Q_DECLARE_METATYPE(Event);

#endif // EVENT_H
