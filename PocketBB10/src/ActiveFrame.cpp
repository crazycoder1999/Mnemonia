/*
 * ActiveFrame.cpp
 *
 *  Created on: 21/giu/2013
 *      Author: nemo
 */

#include "ActiveFrame.h"

namespace cesare {


ActiveFrame::ActiveFrame() :SceneCover(this) {

    // Load the QML for the cover and retrieve the root container
    qml = QmlDocument::create("asset:///AppCover.qml").parent(this);
    mMainContainer = qml->createRootObject<Container>();

    setContent(mMainContainer);

    update();
}

ActiveFrame:: ~ ActiveFrame() {
}

void ActiveFrame::update() {
}

} /* namespace cesare */
