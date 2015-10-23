/*
 * MyIndexMapper.cpp
 *
 *  Created on: 9 mar 2012
 *      Author: ostenlund
 */

#include "MyIndexMapper.h"

using namespace bb::cascades;

MyIndexMapper::MyIndexMapper(int index, int count, bool deleted) :
		mIndex(index), mCount(count), mDeleted(deleted) {
}

bool MyIndexMapper::newIndexPath(QVariantList *pNewIndexPath,
		int *pReplacementIndex, const QVariantList &oldIndexPath) const {

	// Unaffected indices
	if (oldIndexPath[0].toInt() < mIndex) {
		(*pNewIndexPath).append(oldIndexPath);
		return true;

	// Deleted indices
	} else if (mDeleted && oldIndexPath[0].toInt() <= mIndex + mCount) {
		*pReplacementIndex = mIndex;
		return false;

	// Indices after a deletion or addition
	} else {
		if (mDeleted)
			(*pNewIndexPath).append(oldIndexPath[0].toInt() - mCount);
		else
			(*pNewIndexPath).append(oldIndexPath[0].toInt() + mCount);
		return true;
	}
}

