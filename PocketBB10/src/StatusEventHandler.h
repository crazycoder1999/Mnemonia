/*
 * StatusEventHandler.h
 *
 *  Created on: 09/giu/2013
 *      Author: nemo
 */

#ifndef STATUSEVENTHANDLER_H_
#define STATUSEVENTHANDLER_H_

#include <QObject>
#include <QString>
#include <bb/AbstractBpsEventHandler>

namespace cesare {

class StatusEventHandler: public QObject, public bb::AbstractBpsEventHandler {
    Q_OBJECT
public:
    StatusEventHandler();
    virtual ~StatusEventHandler();
    virtual void event(bps_event_t *event);

signals:
    void networkStatusUpdated(bool status, QString type);
    void localeUpdated(QString language, QString country, QString locale);

};

} /* namespace cesare */
#endif /* STATUSEVENTHANDLER_H_ */
