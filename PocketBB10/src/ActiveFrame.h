/*
 * ActiveFrame.h
 *
 *  Created on: 21/giu/2013
 *      Author: nemo
 */

#ifndef ACTIVEFRAME_H_
#define ACTIVEFRAME_H_

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>
#include <bb/cascades/QmlDocument>


using namespace bb::cascades;

namespace bb {
	namespace cascades {

		class Container;
		class QmlDocument;
		class QObject;

	}
}

namespace cesare {

class ActiveFrame : public SceneCover {
	Q_OBJECT

public:
    ActiveFrame();
    virtual ~ActiveFrame();
    void update();
private:
    Container *mMainContainer;
    QmlDocument *qml;


};
} /* namespace cesare */





#endif /* ACTIVEFRAME_H_ */
